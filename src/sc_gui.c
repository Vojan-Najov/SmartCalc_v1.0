#include "sc_error.h"
#include "sc_lexer.h"
#include "sc_scanner.h"
#include "sc_parser.h"
#include "sc_calculator.h"
#include "sc_variable.h"

#include <gtk/gtk.h>

static void app_activate_cb(GApplication *app);

static void btn_clicked_cb(GtkButton *btn, gpointer data);

static const char *get_str_from_label(const char *label, \
                                      const char *bufstr, guint buflen);

static void assign_btn_clicked_cb(GtkButton *btn, gpointer data);

static void plot_btn_clicked_cb(GtkButton *btn, gpointer data);

char *handle_user_input(const char *str);

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
	gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));\

	for (size_t i = 0; i < sizeof(button_id_array) / sizeof(const char *); ++i) {
		btn = GTK_WIDGET(gtk_builder_get_object(build, button_id_array[i]));
		g_signal_connect(btn, "clicked", G_CALLBACK(btn_clicked_cb), build);
	}

	btn = GTK_WIDGET(gtk_builder_get_object(build, "assign_button"));
	g_signal_connect(btn, "clicked", G_CALLBACK(assign_btn_clicked_cb), build);

	btn = GTK_WIDGET(gtk_builder_get_object(build, "plot_button"));
	g_signal_connect(btn, "clicked", G_CALLBACK(plot_btn_clicked_cb), build);

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
	GtkAdjustment *vadj;
	GtkTextBuffer *tv_buf;
	GtkTextIter tv_buf_iter;
	GtkEntryBuffer *entry_buf;
	const char *entry_buf_str;
	guint entry_buf_len;

	(void) btn;

	entry = GTK_WIDGET(gtk_builder_get_object(build, "ent"));
	entry_buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
	entry_buf_str = gtk_entry_buffer_get_text(entry_buf);
	entry_buf_len = gtk_entry_buffer_get_length(entry_buf);

	
	char *str = handle_user_input(entry_buf_str);
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
	}
	if (*str == '-' || g_ascii_isdigit(*str)) {
		gtk_entry_buffer_set_text(entry_buf, str, -1);
	} else {
		gtk_entry_buffer_delete_text(entry_buf, 0, entry_buf_len);
	}
	g_free(str);
	
}

char *handle_user_input(const char *str_in) {
	sc_deque_t lexems, rpn;
	char *str_out;
	double result;
	int expr_type;
	int err_status;
	
	sc_init_deque(&lexems);
	err_status = sc_lexer(str_in, &lexems);
	if (err_status) {
		str_out = sc_gui_error_lexer(err_status, &lexems);
		return (str_out);
	} else {
		expr_type = sc_scanner(&lexems);
		if (expr_type == SC_SCANNER_ERROR) {
			str_out = sc_gui_error_scanner(&lexems);
			return (str_out);
		} else {
			sc_init_deque(&rpn);
			err_status = sc_parser(&lexems, &rpn);
			if (err_status) {
				str_out = sc_gui_error_parser(err_status, &lexems, &rpn);
				return (str_out);
			} else {
				if (expr_type == SC_ASSIGNMENT) {
					err_status = sc_assignment(&rpn);
					sc_get_variable(&result);
					str_out = err_status ? NULL : g_strdup_printf("%.16g", result);
				} else if (expr_type == SC_DEFINITION) {
					err_status = sc_definition(&rpn);
					str_out = err_status ? NULL : g_strdup("definition of the f(x)");
				} else if (expr_type == SC_EXPRESSION) {
					err_status = sc_calculation(&rpn, &result);
					str_out = err_status ? NULL : g_strdup_printf("%.16g", result);
				}
				if (err_status) {
					str_out = sc_gui_error_calculator(err_status, &rpn);
				} else {
					rpn.clear(&rpn);
				}
			}
		}
	}

	return (str_out);
}

static void plot_btn_clicked_cb(GtkButton *btn, gpointer data) {
	static int plot_open = 0;
	GtkWidget *plot_boxv;
	GtkWidget *win;
	GtkStyleContext *btn_context;
	GtkBuilder *build = GTK_BUILDER(data);

	if (!plot_open) {
		plot_boxv = GTK_WIDGET(gtk_builder_get_object(build, "plot_boxv"));
		gtk_widget_set_visible(plot_boxv, TRUE);
		btn_context = gtk_widget_get_style_context(GTK_WIDGET(btn));
		gtk_style_context_add_class(btn_context, "suggested-action");
		plot_open = 1;
	} else {
		plot_boxv = GTK_WIDGET(gtk_builder_get_object(build, "plot_boxv"));
		gtk_widget_set_visible(plot_boxv, FALSE);
		win = GTK_WIDGET(gtk_builder_get_object(build, "win"));
		gtk_window_set_default_size(GTK_WINDOW(win), 400, 500);
		btn_context = gtk_widget_get_style_context(GTK_WIDGET(btn));
		gtk_style_context_remove_class(btn_context, "suggested-action");
		plot_open = 0;
	}
}
/*
static void plot_btn_clicked_cb(GtkButton *btn, gpointer data) {
	static int plot_open = 0;
	static GtkWidget *box;
	static GtkWidget *lbl;
	GtkWidget *win;
	GtkStyleContext *btn_context;
	GtkBuilder *build = GTK_BUILDER(data);

	if (!plot_open) {
		box = GTK_WIDGET(gtk_builder_get_object(build, "main_boxh"));
		lbl = gtk_label_new("PLOT");
		gtk_box_append(GTK_BOX(box), lbl);
		btn_context = gtk_widget_get_style_context(GTK_WIDGET(btn));
		gtk_style_context_add_class(btn_context, "suggested-action");
		plot_open = 1;
	} else {
		gtk_box_remove(GTK_BOX(box), lbl);
		win = GTK_WIDGET(gtk_builder_get_object(build, "win"));
		gtk_window_set_default_size(GTK_WINDOW(win), 400, 500);
		btn_context = gtk_widget_get_style_context(GTK_WIDGET(btn));
		gtk_style_context_remove_class(btn_context, "suggested-action");
		plot_open = 0;
	}
}
*/
