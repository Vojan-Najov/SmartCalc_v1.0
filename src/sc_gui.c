#include "sc_error.h"
#include "sc_lexer.h"
#include "sc_scanner.h"
#include "sc_parser.h"
#include "sc_calculator.h"
#include "sc_function.h"
#include "sc_variable.h"

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

static void app_activate_cb(GApplication *app);

static void btn_clicked_cb(GtkButton *btn, gpointer data);

static const char *get_str_from_label(const char *label, \
                                      const char *bufstr, guint buflen);

static void assign_btn_clicked_cb(GtkButton *btn, gpointer data);

static char *handle_user_input(const char *src);

static void plot_btn_clicked_cb(GtkButton *btn, gpointer data);

static void construct_btn_clicked_cb(GtkButton *btn, gpointer data);

static void draw_function(GtkDrawingArea *area, cairo_t *cr,
                          int width, int height, gpointer data);

static void drawing_coordinate_axes(cairo_t *cr, int width, int height);

static void get_df_ef(GtkBuilder *build, double *dmin, double *dmax, \
                      double *emin,  double *emax);

static void drawing_plot(cairo_t *cr, int width, int height, \
                         double dmin, double dmax, double emin, double emax);

static void drawing_plot_cycle(cairo_t *cr_src, double dmin, double dmax,
                               double step, sc_affine_transform_t *transform);

static void drawing_adaptive_grid(cairo_t *cr_src, int width, int height, \
                         double dmin, double dmax, double emin, double emax);

static void drawing_scale_x(cairo_t *cr, int width, int height,
                          double dmin, double dmax, double emin, double emax);

static void drawing_scale_y(cairo_t *cr, int width, int height,
                          double dmin, double dmax, double emin, double emax);

static double transformation_x(sc_affine_transform_t *transform, double x);

static double transformation_y(sc_affine_transform_t *transform, double y);


int smartcalc_gui(int argc, char **argv) {
	GtkApplication *app;
	int status;

	app = gtk_application_new("com.github.Vojan-Najov.SmartCalc_v1-0",
                              G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate_cb), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return (status);
}

static void app_activate_cb(GApplication *app) {
	GtkBuilder *build;
	GtkWidget *win;
	GtkWidget *btn;
	const char *button_id_array[] = {
		"clear_button", "lbracket_button", "rbracket_button",
		"button0", "button1", "button2",
		"button3", "button4", "button5",
		"button6", "button7", "button8",
		"button9", "dot_button", "plus_button",
		"minus_button", "mul_button", "div_button",
		"pow_button", "mod_button", "cos_button",
		"sin_button", "tan_button", "acos_button",
		"asin_button", "atan_button", "sqrt_button",
		"ln_button", "log_button", "x_button",
		"f_button", "x_assignment_button", "f_definition_button",
	};

	build = gtk_builder_new_from_file("./resources/test.ui");

	win = GTK_WIDGET(gtk_builder_get_object(build, "win"));
	gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));

	for (size_t i = 0; i < sizeof(button_id_array) / sizeof(const char *); ++i) {
		btn = GTK_WIDGET(gtk_builder_get_object(build, button_id_array[i]));
		g_signal_connect(btn, "clicked", G_CALLBACK(btn_clicked_cb), build);
	}

	btn = GTK_WIDGET(gtk_builder_get_object(build, "assign_button"));
	g_signal_connect(btn, "clicked", G_CALLBACK(assign_btn_clicked_cb), build);

	btn = GTK_WIDGET(gtk_builder_get_object(build, "plot_button"));
	g_signal_connect(btn, "clicked", G_CALLBACK(plot_btn_clicked_cb), build);

	btn = GTK_WIDGET(gtk_builder_get_object(build, "construct_graph_button"));
	g_signal_connect(btn, "clicked", G_CALLBACK(construct_btn_clicked_cb), build);

	GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(build, "draw"));
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(area), 800);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(area), 800);
	gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area),
                                   draw_function, build, NULL);

	gtk_window_present(GTK_WINDOW(win));
}

static void btn_clicked_cb(GtkButton *btn, gpointer data) {
	GtkBuilder *build = GTK_BUILDER(data);
	GtkWidget *entry;
	GtkEntryBuffer *entry_buf;
	const char *bufstr;
	const char *label;
	const char *str;
	guint buflen;

	label = gtk_button_get_label(btn);
	entry = GTK_WIDGET(gtk_builder_get_object(build, "ent"));
	entry_buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
	bufstr = gtk_entry_buffer_get_text(entry_buf);
	buflen = gtk_entry_buffer_get_length(entry_buf);

	if (g_strcmp0(label, "cl") == 0) {
		gtk_entry_buffer_delete_text(entry_buf, 0, buflen);
	} else {
		 if (g_strcmp0(label, "x assignment") == 0) {
			gtk_entry_buffer_delete_text(entry_buf, 0, buflen);
			str = "x = ";
		} else if (g_strcmp0(label, "f definition") == 0) {
			gtk_entry_buffer_delete_text(entry_buf, 0, buflen);
			str = "f = ";
		} else {
			str = get_str_from_label(label, bufstr, buflen);
		}
		gtk_entry_buffer_insert_text(entry_buf, buflen, str, strlen(str));
	}
}

static const char *get_str_from_label(const char *label, \
                                      const char *bufstr, guint buflen) {
	const char *str;

	if (g_strcmp0(label, "mod") == 0) {
		str = " mod ";
	} else if (g_strcmp0(label, "cos") == 0) {
		str = "cos(";
	} else if (g_strcmp0(label, "sin") == 0) {
		str = "sin(";
	} else if (g_strcmp0(label, "tan") == 0) {
		str = "tan(";
	} else if (g_strcmp0(label, "acos") == 0) {
		str = "acos(";
	} else if (g_strcmp0(label, "asin") == 0) {
		str = "asin(";
	} else if (g_strcmp0(label, "atan") == 0) {
		str = "atan(";
	} else if (g_strcmp0(label, "sqrt") == 0) {
		str = "sqrt(";
	} else if (g_strcmp0(label, "ln") == 0) {
		str = "ln(";
	} else if (g_strcmp0(label, "log") == 0) {
		str = "log(";
	} else if (g_strcmp0(label, "f") == 0) {
		str = "f(";
	} else if (g_strcmp0(label, ".") == 0) {
		if (!buflen || (buflen > 0 && !g_ascii_isdigit(bufstr[buflen - 1]))) {
			str = "0.";
		} else {
			str = ".";
		}
	} else {
		str = label;
	}

	return (str);
}

static void assign_btn_clicked_cb(GtkButton *btn, gpointer data) {
	GtkBuilder *build = GTK_BUILDER(data);
	GtkWidget *entry;
	GtkWidget *tv;
	GtkWidget *scr;
	GtkWidget *plot_lbl;
	GtkWidget *area;
	GtkAdjustment *vadj;
	GtkTextBuffer *tv_buf;
	GtkTextIter tv_buf_iter;
	GtkEntryBuffer *entry_buf;
	const char *entry_buf_str;
	guint entry_buf_len;
	char *str;

	(void) btn;

	entry = GTK_WIDGET(gtk_builder_get_object(build, "ent"));
	entry_buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
	entry_buf_str = gtk_entry_buffer_get_text(entry_buf);
	entry_buf_len = gtk_entry_buffer_get_length(entry_buf);
	
	str = handle_user_input(entry_buf_str);

	if (*str) {
		tv = GTK_WIDGET(gtk_builder_get_object(build, "tv"));
		tv_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
		gtk_text_buffer_get_end_iter(tv_buf, &tv_buf_iter);
		gtk_text_buffer_insert(tv_buf, &tv_buf_iter, "> ", 2);
		gtk_text_buffer_get_end_iter(tv_buf, &tv_buf_iter);
		gtk_text_buffer_insert(tv_buf, &tv_buf_iter, entry_buf_str, entry_buf_len);
		gtk_text_buffer_get_end_iter(tv_buf, &tv_buf_iter);
		gtk_text_buffer_insert(tv_buf, &tv_buf_iter, "\n", 1);
		if (*str == '-' || g_ascii_isdigit(*str)) {
			gtk_text_buffer_get_end_iter(tv_buf, &tv_buf_iter);
			gtk_text_buffer_insert(tv_buf, &tv_buf_iter, "= ", 2);
		}
		gtk_text_buffer_get_end_iter(tv_buf, &tv_buf_iter);
		gtk_text_buffer_insert(tv_buf, &tv_buf_iter, str, -1);
		gtk_text_buffer_get_end_iter(tv_buf, &tv_buf_iter);
		gtk_text_buffer_insert(tv_buf, &tv_buf_iter, "\n", 1);

		scr = GTK_WIDGET(gtk_builder_get_object(build, "scr"));
		vadj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scr));
		gtk_adjustment_set_upper(vadj, gtk_adjustment_get_upper(vadj) + 20.0);	
		gtk_adjustment_set_value(vadj, gtk_adjustment_get_upper(vadj));
	}

	if (g_strcmp0(str, "definition of the f(x)") == 0) {
		plot_lbl = GTK_WIDGET(gtk_builder_get_object(build, "plot_lbl"));
		gtk_label_set_text(GTK_LABEL(plot_lbl), entry_buf_str);
		area = GTK_WIDGET(gtk_builder_get_object(build, "draw"));
		gtk_widget_queue_draw(area);
	}

	if (*str == '-' || g_ascii_isdigit(*str)) {
		gtk_entry_buffer_set_text(entry_buf, str, -1);
	} else {
		gtk_entry_buffer_delete_text(entry_buf, 0, entry_buf_len);
	}

	g_free(str);
}

static char *handle_user_input(const char *src) {
	sc_deque_t lexems, rpn;
	char *dest;
	double result;
	int expr_type;
	int err_status;
	
	sc_init_deque(&lexems);
	err_status = sc_lexer(src, &lexems);
	if (err_status) {
		dest = sc_gui_error_lexer(err_status, &lexems);
	} else {
		expr_type = sc_scanner(&lexems);
		if (expr_type == SC_SCANNER_ERROR) {
			dest = sc_gui_error_scanner(&lexems);
		} else {
			sc_init_deque(&rpn);
			err_status = sc_parser(&lexems, &rpn);
			if (err_status) {
				dest = sc_gui_error_parser(err_status, &lexems, &rpn);
			} else {
				lexems.clear(&lexems);
				if (expr_type == SC_ASSIGNMENT) {
					err_status = sc_assignment(&rpn);
					sc_get_variable(&result);
					dest = err_status ? NULL : g_strdup_printf("%.16g", result);
				} else if (expr_type == SC_DEFINITION) {
					err_status = sc_definition(&rpn);
					dest = err_status ? NULL : g_strdup("definition of the f(x)");
				} else if (expr_type == SC_EXPRESSION) {
					err_status = sc_calculation(&rpn, &result);
					dest = err_status ? NULL : g_strdup_printf("%.16g", result);
				}
				if (err_status) {
					dest = sc_gui_error_calculator(err_status, &rpn);
				} else {
					rpn.clear(&rpn);
				}
			}
		}
	}

	return (dest);
}

static void plot_btn_clicked_cb(GtkButton *btn, gpointer data) {
	static int plot_open = 0;
	GtkWidget *plot_boxv;
	GtkWidget *win;
	GtkStyleContext *btn_context;
	GtkBuilder *build = GTK_BUILDER(data);

	plot_boxv = GTK_WIDGET(gtk_builder_get_object(build, "plot_boxv"));
	btn_context = gtk_widget_get_style_context(GTK_WIDGET(btn));

	if (!plot_open) {
		gtk_widget_set_visible(plot_boxv, TRUE);
		gtk_style_context_add_class(btn_context, "suggested-action");
		plot_open = 1;
	} else {
		gtk_widget_set_visible(plot_boxv, FALSE);
		win = GTK_WIDGET(gtk_builder_get_object(build, "win"));
		gtk_window_set_default_size(GTK_WINDOW(win), 400, 500);
		gtk_style_context_remove_class(btn_context, "suggested-action");
		plot_open = 0;
	}
}

static void construct_btn_clicked_cb(GtkButton *btn, gpointer data) {
	GtkBuilder *build = GTK_BUILDER(data);
	GtkWidget *area;

	(void) btn;
	area = GTK_WIDGET(gtk_builder_get_object(build, "draw"));
	gtk_widget_queue_draw(area);
}

static void draw_function(GtkDrawingArea *area, cairo_t *cr,
                          int width, int height, gpointer data) {
	GtkBuilder *build = GTK_BUILDER(data);
	double dmin, dmax, emin, emax;
	(void) area;

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_paint_with_alpha(cr, 0.1);

	get_df_ef(build, &dmin, &dmax, &emin, &emax);

	drawing_coordinate_axes(cr, width, height);
	
	drawing_adaptive_grid(cr, width, height, dmin, dmax, emin, emax);

	drawing_plot(cr, width, height, dmin, dmax, emin, emax);

	drawing_scale_x(cr, width, height, dmin, dmax, emin, emax);
	drawing_scale_y(cr, width, height, dmin, dmax, emin, emax);
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

	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_select_font_face (cr, "Georgia",
                            CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 15);

	cairo_text_extents (cr, "x", &te);
	cairo_move_to(cr, (double)width - te.width - 2, (double)height/2.0 - 4);
	cairo_show_text (cr, "x");

	cairo_text_extents (cr, "f(x)", &te);
	cairo_move_to(cr, (double)width/2.0 - te.width - 4, te.height);
	cairo_show_text (cr, "f(x)");
}

static void get_df_ef(GtkBuilder *build, double *dmin, double *dmax, \
                      double *emin,  double *emax) {
	GtkWidget *entry;
	GtkEntryBuffer *entry_buf;
	const char *bufstr;
	double num;

	entry = GTK_WIDGET(gtk_builder_get_object(build, "df_min"));
	entry_buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
	bufstr = gtk_entry_buffer_get_text(entry_buf);
	if (!*bufstr && (*bufstr != '-' || !g_ascii_isdigit(*bufstr))) {
		num = -10.0;
	} else {
		num = g_ascii_strtod(bufstr, NULL);
		if (num > 1.0E6) {
			num = 1.0E6;
		} else if (num < -1.0E6) {
			num = -1.0E6;
		}
	}
	*dmin = num;

	entry = GTK_WIDGET(gtk_builder_get_object(build, "df_max"));
	entry_buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
	bufstr = gtk_entry_buffer_get_text(entry_buf);
	if (!*bufstr && (*bufstr != '-' || !g_ascii_isdigit(*bufstr))) {
		num = 10.0;
	} else {
		num = g_ascii_strtod(bufstr, NULL);
		if (num > 1.0E6) {
			num = 1.0E6;
		} else if (num < -1.0E6) {
			num = -1.0E6;
		}
	}
	*dmax = num;

	entry = GTK_WIDGET(gtk_builder_get_object(build, "ef_min"));
	entry_buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
	bufstr = gtk_entry_buffer_get_text(entry_buf);
	if (!*bufstr && (*bufstr != '-' || !g_ascii_isdigit(*bufstr))) {
		num = -10.0;
	} else {
		num = g_ascii_strtod(bufstr, NULL);
		if (num > 1.0E6) {
			num = 1.0E6;
		} else if (num < -1.0E6) {
			num = -1.0E6;
		}
	}
	*emin = num;

	entry = GTK_WIDGET(gtk_builder_get_object(build, "ef_max"));
	entry_buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
	bufstr = gtk_entry_buffer_get_text(entry_buf);
	if (!*bufstr && (*bufstr != '-' || !g_ascii_isdigit(*bufstr))) {
		num = 10.0;
	} else {
		num = g_ascii_strtod(bufstr, NULL);
		if (num > 1.0E6) {
			num = 1.0E6;
		} else if (num < -1.0E6) {
			num = -1.0E6;
		}
	}
	*emax = num;
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
	cairo_translate(cr, source, height / 2);
	cairo_rotate(cr, M_PI / 2);
	cairo_move_to(cr, 0, 0);
	cairo_show_text (cr, str);
	cairo_restore(cr);

	source = width / 2 + 40;
	for (double tmp = d + delta; source <= width; tmp += delta, source += 40) {
		sprintf(str, "%.2g", tmp);
		cairo_save(cr);
		cairo_text_extents (cr, str, &te);
		cairo_translate(cr, source - te.height, height / 2);
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
		cairo_translate(cr, source + te.height, height / 2);
		cairo_text_extents (cr, str, &te);
		cairo_rotate(cr, -M_PI / 2.0);
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
	cairo_move_to(cr, width / 2, source);
	cairo_show_text (cr, str);

	source = height / 2 - 40;
	for (double tmp = d + delta; source >= 0; tmp += delta, source -= 40) {
		sprintf(str, "%.2g", tmp);
		cairo_text_extents (cr, str, &te);
		cairo_move_to(cr, width/2, source + te.height);
		cairo_show_text (cr, str);
	}

	source = height / 2 + 40;
	for (double tmp = d - delta; source <= height; tmp -= delta, source += 40) {
		sprintf(str, "%.2g", tmp);
		cairo_text_extents (cr, str, &te);
		cairo_move_to(cr, width/2 - te.width, source);
		cairo_show_text (cr, str);
	}
}

