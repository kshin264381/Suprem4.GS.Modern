#!/usr/bin/env python
# -*- coding: utf-8 -*-

#####################################
#                                   #
# process_str.py                    #
#                                   #
# Generate plots and saves *.str    #
# file data to more friendly format #
# such as xlsx.                     #
#                                   #
# Taylor Shin, Apr 07 2016          #
#                                   #
#####################################

import os
import sys

import xlsxwriter as xw
import xlwt as xlw
import matplotlib
try:
    matplotlib.use('Qt4Agg')
except ValueError:
    print("Cannot use Qt4Agg, might not freeze...")
import matplotlib.pyplot as plt


"""@This part handles plotting with matplotlib
"""
class MatPlotLibPlotter(object):
    """@Constructor
    """
    def __init__(self, NodeList, Dimension, input_file, disp_dopant=[]):
        self.dimension = Dimension
        self.input_data = NodeList
        self.input_str_file = input_file
        self.dopant_list = self.input_data[0].GetDopants()
        if not disp_dopant:
            self.disp_dopant = self.dopant_list
        else:
            self.disp_dopant = disp_dopant

        # titalize display dopants.
        if self.disp_dopant:
            tmp = [dd.title() for dd in self.disp_dopant]
            self.disp_dopant = tmp

        if self.dimension > 4:
            raise ValueError("We support only 1D, 2D, and (might be) 3D...")
            sys.exit(-1)

        self.display_mat()

    """@Process display
    """
    def display_mat(self):
        if self.dimension is 1:
            plt.figure()
            try:
                num_o_dopants = self.input_data[0].GetNumOImpurities()
            except ValueError:
                raise ValueError("Cannot determine the number of dopants...")
                sys.exit(-1)
            # extract data
            x_axis = []
            #dopant_data = [[] for n in range(num_o_dopants)]
            if not self.disp_dopant:
                dopant_data = [[] for n in range(num_o_dopants)]
            else:
                dopant_data = [[] for dd in self.disp_dopant]

            for inp_dat in self.input_data:
                # Coordinate
                x_axis.append(inp_dat.GetCoord()[0])
                # Dopant data
                for n, d in enumerate(self.disp_dopant):
                    dci = self.dopant_list.index(d)
                    dopant_data[n].append(inp_dat.impurity.GetDopConc(dci+1))

            # Draw normal plot
            dopant_label = [dop for dop in self.dopant_list]
            for n_i, d in enumerate(self.disp_dopant):
                dci = self.dopant_list.index(d)
                dopant_label[dci] = \
                    inp_dat.impurity.GetDopants(dci+1)
                plt.semilogy(
                    x_axis, dopant_data[n_i], "--",
                    label=dopant_label[dci])

            plt.legend()
            plt.title(self.input_str_file)
            plt.xlabel(r"Depth from surface ($\mu m$)")
            plt.ylabel(r"Concentration ($cm^{-3}$)")

        elif self.dimension is 2:
            # Draw contour plot only with matplolib

            # Import matplotlib stuff for contour plots.
            from matplotlib import colors, ticker, cm
            # import matplotlib.mlab as mlab

            # Setup Data container
            Data = DataSort(self.input_data)

            # Setting up X, Y mesh
            x_list, y_list = Data.GetCoords()
            X, Y = MatPlotLibPlotter.make_mesh(x_list, y_list)
            #dopant_list = Data.GetImpuritySet()
            Doping_Conc_Table = {}
            plottable = True
            for dl in self.disp_dopant:
                fig = plt.figure()
                Doping_Conc_Table[dl] = Data.GetConcList(dl)
                try:
                    CS = plt.contourf(X, Y, Doping_Conc_Table[dl],
                        locator=ticker.LogLocator(),
                        cmap=cm.PuBu_r)
                    CBAR = plt.colorbar()
                    plottable = True
                except ValueError:
                    plottable = False
                    plt.close()

                if plottable:
                    plt.gca().invert_yaxis()
                    plt.gca().xaxis.tick_top()
                    fig.canvas.set_window_title(self.input_str_file+' - '+dl)
                    plt.title(dl, y=-0.08)
                    plt.xlabel(r"Width ($\mu$ m)")
                    plt.gca().xaxis.set_label_position('top')
                    plt.ylabel(r"Height ($\mu$ m)")

        else:
            raise ValueError("Ouch, Suprem4.GS doesn't support 3D!!")
            sys.exit(-1)

        plt.show()

    """@A dirt quick method to eliminate usage of numpy.
    --> Generates meshgrid(contour plots) for x, y ranges.
    """
    @staticmethod
    def make_mesh(x_list, y_list):
        return [x_list for y_l in y_list], \
            [[y_list[iy] for x in x_list] for iy, y in enumerate(y_list)]

"""@This part handles xls file generation
"""
class SaveAsXls(object):
    """@Constructor
    """
    def __init__(self, NodeList, dimension, in_filename):
        self.input_data = NodeList
        self.out_filename = os.path.basename(in_filename)+".xls"
        self.dimension = dimension

        # Main data storage
        self.xlsDataSet = {}

        # Writes data to storage and save them to xlsx file.
        self.saveToXls()

    """@Save stuffs to xls file
    """
    def saveToXls(self):
        if self.dimension == 1:
            self.Save1DXls()

        elif self.dimension == 2:
            self.Save2DXls()

        print("Workbook Saved: %s" % self.out_filename)

    """@Save 1D data to xlsx
    """
    def Save1DXls(self):
        # Open up the workbook
        wb = xlw.Workbook(self.out_filename)

        # save 1D data
        DataCont = DataSort(self.input_data)

        x_list = DataCont.GetCoords()
        self.xlsDataSet['1d_coords'] = x_list

        dopant_list = DataCont.GetImpuritySet()

        # populate keys at xlsxDataSet
        for dl in dopant_list:
            self.xlsDataSet[dl] = []

        # Now put in dataset
        for key in dopant_list:
            for x in self.xlsDataSet['1d_coords']:
                self.xlsDataSet[key].append(DataCont.GetConc(x, key))

        # Ok, lets write those stuff to the workbook!!
        for dl in dopant_list:
            ws = wb.add_sheet(dl)
            ws.col(0).width = 12*367
            ws.col(1).width = 30*367
            ws.write(0, 0, "Depth (um)")
            ws.write(0, 1, "Concentration (cm^-3)")
            for ix, x in enumerate(self.xlsDataSet['1d_coords']):
                ws.write(ix+1, 0, x)
                ws.write(ix+1, 1, self.xlsDataSet[dl][ix])

        # Ok, save the workbook
        wb.save(self.out_filename)

    """@Save 2D data to xlsx
    """
    def Save2DXls(self):
        # Open up the workbook
        wb = xlw.Workbook(self.out_filename)

        # Ok, prepare 2D dataset
        Data = DataSort(self.input_data)
        # X, Y coordinate set
        coord_list = Data.GetCoords()
        # Dopant list
        dopant_list = Data.GetImpuritySet()

        # Populate self.xlsxDataSet
        for dl in dopant_list:
            self.xlsDataSet[dl] = {}

        # Now fill in self.xlsxDataSet with doping concentration
        x_list = coord_list[0]
        y_list = coord_list[1]
        for xc in x_list:
            for yc in y_list:
                current_coord = (xc, yc)
                for dl in dopant_list:
                    self.xlsDataSet[dl][current_coord] = \
                        float(Data.GetConc(current_coord, dl))

        # Ok, now... let's write the xlsx file...
        for dl in dopant_list:
            # add a new worksheet per dopant...
            ws = wb.add_sheet(dl)
            ws.write(0, 0, "Column: Y (um) / Row: X (um)")
            # populste coordinates
            for ix, x in enumerate(x_list):
                ws.write(0, ix+1, x)
            for iy, y in enumerate(y_list):
                ws.write(iy+1, 0, y)
            # Ok, now populate the doping concentrations.
            for ixc, xc in enumerate(x_list):
                for iyc, yc in enumerate(y_list):
                    ws.write(iyc+1, ixc+1, 
                        self.xlsDataSet[dl][(xc, yc)])

        # Ok, save the workbook
        wb.save(self.out_filename)

"""@This part handles xlsx file generation
"""
class SaveAsXlsx(object):
    """@Constructor
    """
    def __init__(self, NodeList, dimension, in_filename):
        self.input_data = NodeList
        self.out_filename = os.path.basename(in_filename)+".xlsx"
        self.dimension = dimension

        # Main data storage
        self.xlsxDataSet = {}

        # Writes data to storage and save them to xlsx file.
        self.saveToXlsx()

    """@Save stuffs to xlsx file
    """
    def saveToXlsx(self):
        if self.dimension == 1:
            self.Save1DXlsx()

        elif self.dimension == 2:
            self.Save2DXlsx()

        print("Workbook Saved: %s" % self.out_filename)

    """@Save 1D data to xlsx
    """
    def Save1DXlsx(self):
        # Open up the workbook
        wb = xw.Workbook(self.out_filename)

        # save 1D data
        DataCont = DataSort(self.input_data)

        x_list = DataCont.GetCoords()
        self.xlsxDataSet['1d_coords'] = x_list

        dopant_list = DataCont.GetImpuritySet()

        # populate keys at xlsxDataSet
        for dl in dopant_list:
            self.xlsxDataSet[dl] = []

        # Now put in dataset
        for key in dopant_list:
            for x in self.xlsxDataSet['1d_coords']:
                self.xlsxDataSet[key].append(DataCont.GetConc(x, key))

        # Ok, lets write those stuff to the workbook!!
        for dl in dopant_list:
            ws = wb.add_worksheet(dl)
            ws.set_column("A:A", 12)
            ws.set_column("B:B", 30)
            ws.write(0, 0, "Depth (um)")
            ws.write(0, 1, "Concentration (cm^-3)")
            for ix, x in enumerate(self.xlsxDataSet['1d_coords']):
                ws.write(ix+1, 0, x)
                ws.write(ix+1, 1, self.xlsxDataSet[dl][ix])

        # Now, don't forget to close the workbook
        wb.close()

    """@Save 2D data to xlsx
    """
    def Save2DXlsx(self):
        # Open up the workbook
        wb = xw.Workbook(self.out_filename)

        # Ok, prepare 2D dataset
        Data = DataSort(self.input_data)
        # X, Y coordinate set
        coord_list = Data.GetCoords()
        # Dopant list
        dopant_list = Data.GetImpuritySet()

        # Populate self.xlsxDataSet
        for dl in dopant_list:
            self.xlsxDataSet[dl] = {}

        # Now fill in self.xlsxDataSet with doping concentration
        x_list = coord_list[0]
        y_list = coord_list[1]
        for xc in x_list:
            for yc in y_list:
                current_coord = (xc, yc)
                for dl in dopant_list:
                    self.xlsxDataSet[dl][current_coord] = \
                        float(Data.GetConc(current_coord, dl))

        # Ok, now... let's write the xlsx file...
        for dl in dopant_list:
            # add a new worksheet per dopant...
            ws = wb.add_worksheet(dl)
            ws.write(0, 0, "Column: Y (um) / Row: X (um)")
            # populste coordinates
            for ix, x in enumerate(x_list):
                ws.write(0, ix+1, x)
            for iy, y in enumerate(y_list):
                ws.write(iy+1, 0, y)
            # Ok, now populate the doping concentrations.
            for ixc, xc in enumerate(x_list):
                for iyc, yc in enumerate(y_list):
                    ws.write(iyc+1, ixc+1, 
                        self.xlsxDataSet[dl][(xc, yc)])

        # Now, don't forget to close the workbook
        wb.close()

"""@Data Sorter - Sorts out data from Node class list
"""
class DataSort(object):
    """@Constructor
    """
    def __init__(self, NodeList):
        if not NodeList:
            raise ValueError("Error!! Empty Node list given!!")
        else:
            self.node_list = NodeList

        # Initialize coordinate data space
        self.coordinate_set = []
        self.plot_coordinate_set = []

        # Extract impurity set list and initialize Impurity dataset
        self.Impurity_set = self.node_list[0].GetDopants()
        self.Impurity_data = {}
        for dop in self.Impurity_set:
            self.Impurity_data[dop] = []

        # Determining dimension
        tmp_coord = list(self.node_list[0].GetCoord())
        self.Dimension = len(tmp_coord)

        self.extract_data()

    """@Extract data from nodes and put them into the class
    """
    def extract_data(self):
        self.coordinate_set = []
        for n_i, node in enumerate(self.node_list):
            if self.Dimension == 1:
                self.coordinate_set.append(float(node.GetCoord()[0]))
            else:
                n_coord = node.GetCoord()
                tmp_coord = [float(n) for n in n_coord]
                self.coordinate_set.append(tuple(tmp_coord))

            for idop, dop in enumerate(self.Impurity_set):
                self.Impurity_data[dop].append(node.GetDopConc(idop+1))

    """@Returns impurity concentration from specific coordinate
    """
    def GetConc(self, Coord, Dopant):
        try:
            coord_ind = self.coordinate_set.index(Coord)
        except ValueError:
            return 0

        dop_conc = self.Impurity_data[Dopant][coord_ind]

        return dop_conc

    """@Returns doping concentration data set for plotting
    --> must feed dopant type as string...
    """
    def GetConcList(self, dopant):
        if self.Dimension is 1:
            return [self.GetConc(x, dopant) for c in self.coordinate_set]
        elif self.Dimension is 2:
            x_list, y_list = self.GetCoords()
            Dop_Conc = []
            for yc in y_list:
                x_dop_lst = []
                for xc in x_list:
                    coord = (xc, yc)
                    doping_conc = self.GetConc(coord, dopant)
                    x_dop_lst.append(doping_conc)
                Dop_Conc.append(x_dop_lst)
            return Dop_Conc

    """@Returns coordinate set for plotting
    """
    def GetCoords(self):
        if self.Dimension == 1:
            return self.coordinate_set
        elif self.Dimension == 2:
            # Planarize tuple based coordinate set.
            x_list = []
            y_list = []
            for coord in self.coordinate_set:
                x_list.append(coord[0])
                y_list.append(coord[1])

            # remove duplicates
            x_list = sorted(set(x_list))
            y_list = sorted(set(y_list))

            return [x_list, y_list]

        else:
            raise ValueError(
                "Oh crap, Suprem4.GS can't even calculate 3D process!!")
            sys.exit(-1)

    """@Returns Dopant List
    """
    def GetImpuritySet(self):
        return self.Impurity_set
