#!/usr/bin/env python
# -*- coding: utf-8 -*-

#####################################
#                                   #
# strread.py                        #
#                                   #
# Reads in *.str file and plot      #
# net doping concentration (/cm3)   #
# along with given plane            #
#                                   #
# Taylor Shin, Apr 05 2016          #
#                                   #
#####################################

import os
import sys
import getopt

from process_str import Reader as read_str
from plot_str import MatPlotLibPlotter as mptPlotter
from plot_str import SaveAsXlsx as save_xlsx
from plot_str import SaveAsXls as save_xls

Version = "0.0.0.2"
Title = "  Structure reader "+Version+"\n"+\
    "  *.str file reader & visualizer for Suprem4.GS "

"""@The main function
"""
def main(argv):
    try:
        opts, args = \
            getopt.getopt(argv, "i:d:o:D:", ["help", "grammar="])
    except getopt.GetoptError:
        usage()
        sys.exit(2)

    # Intro screen(?)
    Title_screen = \
        "=========================================================\n"+\
        Title+"\n"+\
        "=========================================================\n"

    print(Title_screen)

    # Parse options
    input_file = ''
    display_option = ''
    output_file = ''
    dopant_option = []

    for op, arg in opts:
        if op == '-i':
            input_file = arg
        elif op == '-d':
            display_option = arg
        elif op == '-o':
            output_type = arg
        elif op == '-D':
            dopant_option.append(arg)

    print(dopant_option)

    # Check if the input file was given without '-i' phrase
    if not input_file:
        if list(args):
            input_file = args[0]
        else:
            usage()
            sys.exit(2)

    # Ok, set up output filename
    if input_file:
        output_file = \
            os.path.basename(input_file).split('.')[0]

    # Setup default options
    if not display_option:
        display_option = 'matplotlib'
    if not output_type:
        output_type = 'xlsx'

    # Ok, read in the str file
    StrReader = read_str(input_file)

    # Display part
    if display_option == 'matplotlib':
        mptPlotter(StrReader.GetNodes(),
            StrReader.Dimension,
            input_file, dopant_option)
    elif display_option == 'nodisp':
        pass

    # Data store part
    if output_type.lower() == 'xlsx':
        save_xlsx(StrReader.GetNodes(), StrReader.Dimension, output_file)
    elif output_type.lower() == 'xls':
        save_xls(StrReader.GetNodes(), StrReader.Dimension, output_file)


    return 0


"""@showing usage
"""
def usage():
    basic_usage = \
        "Usage: %s -i <input_str_file> -d <display_type> -o <output_file>" % sys.argv[0]

    print(basic_usage)


"""@Start the program!!
"""
if __name__ == "__main__":
    main(sys.argv[1:])
    sys.exit(0)
