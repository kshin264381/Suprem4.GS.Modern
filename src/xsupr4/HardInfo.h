#ifndef __HARDINFO_H__
#define __HARDINFO_H__

#define DEVICE 1
#define FILEN 2

#define PS_ 1
#define IDRAW 2
#define HPGL 3

typedef struct print_info {
    Widget dev_file;
    Widget dev_type;
    Widget dev_name;
    GraphWidget graph;
    Widget print_shell;
    Widget do_include;
} print_calldata;


#define MFNAME 25

typedef enum hard_dev_docu_defn { NONE, NO, YES } hard_dev_docu;

//int dev_init(FILE* file, int final_w, int final_h, char* title_font, double title_size, char* axis_font, double axis_size, int nflags, xgOut* this_dev_info, char* user_info, char* err);

typedef struct hard_dev {
    char *dev_name;		/* Device name                */
#ifdef __cplusplus
    //int (*dev_init)(FILE*, int, int, char*, double, char*, double, int, xgOut*, Widget*, char*);		/* Initialization function    *
#else
    int (*dev_init)();        /* Initialization function    */
#endif
    char *dev_spec;		/* Default pipe program       */
    char dev_file[MFNAME];	/* Default file name          */
    char dev_printer[MFNAME];	/* Default printer name       */
    double dev_max_dim;		/* Default maximum dimension (cm)    */
    char dev_title_font[MFNAME];/* Default name of title font        */
    double dev_title_size;	/* Default size of title font (pnts) */
    char dev_axis_font[MFNAME];	/* Default name of axis font         */
    double dev_axis_size;	/* Default size of axis font (pnts)  */
    hard_dev_docu dev_docu;	/* Document predicate                */
} hard_dev;

#endif /* __HARDINFO_H__ */
