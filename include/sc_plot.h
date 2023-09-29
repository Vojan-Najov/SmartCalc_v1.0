#ifndef __SMARTCALC_PLOT_H__
#define __SMARTCALC_PLOT_H__

#include <gtk/gtk.h>

typedef struct _sc_affine_transform {
  double x0;
  double x1;
  double x2;
  double y0;
  double y1;
  double y2;
  double (*transform_x)(struct _sc_affine_transform *, double);
  double (*transform_y)(struct _sc_affine_transform *, double);
} sc_affine_transform_t;

void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height,
                   gpointer data);

#endif
