#include "sc_error.h"
#include "sc_calculator.h"
#include "sc_function.h"
#include "sc_variable.h"
#include "sc_plot.h"

static const char *entry_lbl[] = {
	"df_min", "df_max",
	"ef_min", "ef_max"
};

enum values {DF_MIN = 0, DF_MAX = 1, EF_MIN = 2, EF_MAX = 3};


static void get_df_ef(GtkBuilder *build, double *dmin, double *dmax,
                      double *emin,  double *emax);

static void drawing_coordinate_axes(cairo_t *cr, int width, int height);

static void drawing_plot(cairo_t *cr,
                         int width, int height,
                         double dmin, double dmax,
                         double emin, double emax);

static void drawing_plot_cycle(cairo_t *cr_src,
                               double dmin, double dmax, double step,
                               sc_affine_transform_t *transform);

static void drawing_adaptive_grid(cairo_t *cr_src,
                                  int width, int height,
                                  double dmin, double dmax,
                                  double emin, double emax);

static void drawing_scale_x(cairo_t *cr,
                            int width, int height,
                            double dmin, double dmax,
                            double emin, double emax);

static void drawing_scale_y(cairo_t *cr,
                            int width, int height,
                            double dmin, double dmax,
                            double emin, double emax);

static double transformation_x(sc_affine_transform_t *transform, double x);

static double transformation_y(sc_affine_transform_t *transform, double y);

static void init_affine_transform(sc_affine_transform_t *transform,
                                  double x0, double x1, double x2,
                                  double y0, double y1, double y2);


void draw_function(GtkDrawingArea *area, cairo_t *cr,
                          int width, int height, gpointer data) {
	GtkBuilder *build = GTK_BUILDER(data);
	double dmin, dmax, emin, emax;
	const char *err_str = "Error: invalid values for D(f) or E(f)!";

	(void) area;

	get_df_ef(build, &dmin, &dmax, &emin, &emax);

	if (dmax > dmin && emax > emin) {
		drawing_coordinate_axes(cr, width, height);
		drawing_adaptive_grid(cr, width, height, dmin, dmax, emin, emax);
		drawing_plot(cr, width, height, dmin, dmax, emin, emax);
		drawing_scale_x(cr, width, height, dmin, dmax, emin, emax);
		drawing_scale_y(cr, width, height, dmin, dmax, emin, emax);
	} else {
		cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
		cairo_select_font_face (cr, "Georgia",
                                CAIRO_FONT_SLANT_NORMAL,
                                CAIRO_FONT_WEIGHT_BOLD);
		cairo_set_font_size (cr, 20);
		cairo_move_to(cr, 40, 40);
		cairo_show_text (cr, err_str);
	}
}

static void get_df_ef(GtkBuilder *build, double *dmin, double *dmax, \
                      double *emin,  double *emax) {
	GtkWidget *entry;
	GtkEntryBuffer *entry_buf;
	const char *bufstr;
	double num;

	for (size_t i = 0; i < sizeof(entry_lbl)/sizeof(const char *); ++i) {
		entry = GTK_WIDGET(gtk_builder_get_object(build, entry_lbl[i]));
		entry_buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
		bufstr = gtk_entry_buffer_get_text(entry_buf);
		if (!*bufstr && (*bufstr != '-' || !g_ascii_isdigit(*bufstr))) {
			switch ((int)i) {
				case DF_MIN:
				case EF_MIN:
					num = -10.0;
					break;
				case DF_MAX:
				case EF_MAX:
					num = 10.0;
					break;
			}
		} else {
			num = g_ascii_strtod(bufstr, NULL);
			if (num > 1.0E6) {
				num = 1.0E6;
			} else if (num < -1.0E6) {
				num = -1.0E6;
			}
		}
		switch ((int)i) {
			case DF_MIN:
				*dmin = num;
				break;
			case DF_MAX:
				*dmax = num;
				break;
			case EF_MIN:
				*emin = num;
				break;
			case EF_MAX:
				*emax = num;
				break;
		}
		
	}
}

static void drawing_coordinate_axes(cairo_t *cr, int width, int height) {
	cairo_text_extents_t te;

  	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_move_to (cr, width/2, 0);
    cairo_line_to (cr, width/2, height);
    cairo_move_to (cr, 0, height/2); 
    cairo_line_to (cr, width, height / 2);
  	cairo_set_line_width (cr, 2);
  	cairo_stroke (cr);

	cairo_select_font_face (cr, "Georgia",
                            CAIRO_FONT_SLANT_NORMAL,
                            CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 15);

	cairo_text_extents (cr, "x", &te);
	cairo_move_to(cr, (double)width - te.width - 2, (double)height/2.0 - 4);
	cairo_show_text (cr, "x");

	cairo_text_extents (cr, "f(x)", &te);
	cairo_move_to(cr, (double)width/2.0 - te.width - 4, te.height);
	cairo_show_text (cr, "f(x)");
}

static double transformation_x(sc_affine_transform_t *transform, double x) {
	return ((x + transform->x0) * transform->x1 + transform->x2);
}

static double transformation_y(sc_affine_transform_t *transform, double y) {
	return ((y + transform->y0) * transform->y1 + transform->y2);
}

static void init_affine_transform(sc_affine_transform_t *transform,
                                  double x0, double x1, double x2,
                                  double y0, double y1, double y2) {
	transform->x0 = x0;
	transform->x1 = x1;
	transform->x2 = x2;
	transform->y0 = y0;
	transform->y1 = y1;
	transform->y2 = y2;
	transform->transform_x = &transformation_x;
	transform->transform_y = &transformation_y;
}

static void drawing_plot(cairo_t *cr_src, int width, int height, \
                         double dmin, double dmax, double emin, double emax) {
	double dif = dmax - dmin;
	double emax_tmp = emax;
	double emin_tmp = emin;
	double dmax_tmp = dmax;
	double dmin_tmp = dmin;
	double r, wscale, hscale;
	double step;
	sc_affine_transform_t transform;
	cairo_t *cr;
	cairo_surface_t *surface;

	if (emax - emin > dif) {
		dif = emax - emin;
		r = (dif - (dmax - dmin)) / 2.0;
		dmax += r;
		dmin -= r;
	} else {
		r = (dif - (emax - emin)) / 2.0;
		emax += r;
		emin -= r;
	}
	wscale = width / dif;
	hscale = height / dif;
	step = (dmax_tmp - dmin_tmp) / 1.0E5;
	step = step > 1.0E-6 ? step : 1.0E-6;

	if (sc_function_status() == SC_FUNC_SET) {
		sc_save_variable();

		surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    	cr = cairo_create (surface);

		init_affine_transform(&transform,
                              -(dmin + dif / 2.0), wscale, (double)width / 2.0,
                              -(emin + dif / 2.0), -hscale, (double)height / 2.0);

		drawing_plot_cycle(cr, dmin_tmp, dmax_tmp, step, &transform);

		surface = cairo_get_target(cr);
		cairo_set_source_surface(cr_src, surface, 0, 0);
		cairo_rectangle(cr_src, 0, transform.transform_y(&transform, emax_tmp),
                        width, (emax_tmp - emin_tmp) * hscale);
		cairo_fill(cr_src);
		cairo_rectangle(cr_src, 0, transform.transform_y(&transform, emax_tmp),
                        width, (emax_tmp - emin_tmp) * hscale);
		cairo_stroke(cr_src);

		cairo_surface_destroy(surface);
		cairo_destroy(cr);
		sc_restore_variable();
	}
}

static void drawing_plot_cycle(cairo_t *cr, double dmin, double dmax,
                               double step, sc_affine_transform_t *transform) {
	double x, y;
	double x_next, y_next;
	sc_deque_t *func;
	int err_status;
	int move_to = 1;
  	
	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);

	for (x = dmin; x <= dmax; x += step) {
		if (move_to) {
			sc_set_variable(x);
			sc_get_function(&func);
			err_status = sc_calculation(func, &y);
			sc_destroy_deque(func);
		} else {
			x = x_next;
			y = y_next;
		}
		if (err_status == SC_DEVIDE_BY_ZERO || isnan(y)) {
			move_to = 1;
			continue;
		}
		x_next = x + step;
		sc_set_variable(x_next);
		sc_get_function(&func);
		err_status = sc_calculation(func, &y_next);
		sc_destroy_deque(func);
		if (!isnan(y_next) && !err_status) {
			if (move_to) {
				cairo_move_to(cr, transform->transform_x(transform, x),
                              transform->transform_y(transform, y));
				move_to = 0;
			} else {
				cairo_line_to(cr, transform->transform_x(transform, x),
                              transform->transform_y(transform, y));
			}
			if (fabs((y_next - y) / (x_next - x)) > 1e6) {
				move_to = 1;
			}
		} else {
			move_to = 1;
		}
	}

  	cairo_set_line_width(cr, 2);
	cairo_stroke(cr);
}

static void drawing_adaptive_grid(cairo_t *cr_src,
                                  int width, int height,
                                  double dmin, double dmax,
                                  double emin, double emax) {
	double dif = dmax - dmin;
	double emax_tmp = emax;
	double emin_tmp = emin;
	double dmax_tmp = dmax;
	double dmin_tmp = dmin;
	double r, wscale, hscale;
	cairo_t *cr;
	cairo_surface_t *surface;

	if (emax - emin > dif) {
		dif = emax - emin;
		r = (dif - (dmax - dmin)) / 2.0;
		dmax += r;
		dmin -= r;
	} else {
		r = (dif - (emax - emin)) / 2.0;
		emax += r;
		emin -= r;
	}
	wscale = width / dif;
	hscale = height / dif;

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create (surface);
  	cairo_set_source_rgb(cr, 0.909803922,  0.639215686, 0.090196078);
	for (int a = 0; a < width; a += 40) {
		cairo_move_to(cr, a, 0);
		cairo_line_to(cr, a, height);
		cairo_move_to(cr, 0, a);
		cairo_line_to(cr, width, a);
	}
  	cairo_set_line_width (cr, 1);
	cairo_stroke (cr);
	surface = cairo_get_target(cr);
	cairo_set_source_surface(cr_src, surface, 0, 0);
	cairo_rectangle(cr_src, \
                    width/2 + (dmin_tmp - (dmin + dif/2)) * wscale, \
                    height/2 - (emax_tmp - (emin + dif/2)) * hscale, \
                    (dmax_tmp - dmin_tmp) * wscale, (emax_tmp - emin_tmp) * hscale);
	cairo_fill(cr_src);
	cairo_set_source_rgb(cr_src, 0.294117647, 0.0, 0.509803922);
	cairo_rectangle(cr_src, \
                    width/2 + (dmin_tmp - (dmin + dif/2)) * wscale, \
                    height/2 - (emax_tmp - (emin + dif/2)) * hscale, \
                    (dmax_tmp - dmin_tmp) * wscale, (emax_tmp - emin_tmp) * hscale);
	cairo_stroke(cr_src);

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}

static void drawing_scale_x(cairo_t *cr, int width, int height,
                          double dmin, double dmax, double emin, double emax) {
	cairo_text_extents_t te;
	double d, delta;
	int source;
	char str[256];

	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_select_font_face (cr, "Georgia",
                            CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 10);

	delta = (dmax - dmin > emax - emin) ? (dmax - dmin)/20.0 : (emax - emin)/20.0;

	d = (dmin + dmax) / 2.0;
	source = width / 2;
	sprintf(str, "%.2g", d);
	cairo_save(cr);
	cairo_text_extents (cr, str, &te);
	cairo_translate(cr, source, 2 + height / 2);
	cairo_rotate(cr, M_PI / 2);
	cairo_move_to(cr, 0, 0);
	cairo_show_text (cr, str);
	cairo_restore(cr);

	source = width / 2 + 40;
	for (double tmp = d + delta; source <= width; tmp += delta, source += 40) {
		sprintf(str, "%.2g", tmp);
		cairo_save(cr);
		cairo_text_extents (cr, str, &te);
		cairo_translate(cr, source - te.height, 2 + height / 2);
		cairo_rotate(cr, M_PI / 2);
		cairo_move_to(cr, 0, 0);
		cairo_show_text (cr, str);
		cairo_restore(cr);
	}

	source = width / 2 - 40;
	for (double tmp = d - delta; source >= 0; tmp -= delta, source -= 40) {
		sprintf(str, "%.2g", tmp);
		cairo_save(cr);
		cairo_text_extents (cr, str, &te);
		cairo_translate(cr, source , height / 2 - te.width - 2);
		cairo_text_extents (cr, str, &te);
		cairo_rotate(cr, M_PI / 2.0);
		cairo_move_to(cr, 0, 0);
		cairo_show_text (cr, str);
		cairo_restore(cr);
	}
}

static void drawing_scale_y(cairo_t *cr, int width, int height,
                          double dmin, double dmax, double emin, double emax) {
	cairo_text_extents_t te;
	double d, delta;
	int source;
	char str[256];

	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_select_font_face (cr, "Georgia",
                            CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 10);

	delta = (dmax - dmin > emax - emin) ? (dmax - dmin)/20.0 : (emax - emin)/20.0;

	d = (emin + emax) / 2.0;

	source = height / 2;
	sprintf(str, "%.2g", d);
	cairo_text_extents (cr, str, &te);
	cairo_move_to(cr, 2 + width / 2, source);
	cairo_show_text (cr, str);

	source = height / 2 - 40;
	for (double tmp = d + delta; source >= 0; tmp += delta, source -= 40) {
		sprintf(str, "%.2g", tmp);
		cairo_text_extents (cr, str, &te);
		cairo_move_to(cr, 2 + width/2, source + te.height);
		cairo_show_text (cr, str);
	}

	source = height / 2 + 40;
	for (double tmp = d - delta; source <= height; tmp -= delta, source += 40) {
		sprintf(str, "%.2g", tmp);
		cairo_text_extents (cr, str, &te);
		cairo_move_to(cr, width/2 - te.width - 2, source);
		cairo_show_text (cr, str);
	}
}

