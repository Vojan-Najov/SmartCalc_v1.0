#include <gtk/gtk.h>

static const char *tmp(const char *label, const char *bufstr, guint buflen) {
	const char *str;

	if (g_strcmp0(label, "cos") == 0) {
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

static void on_clicked(GtkButton *btn, gpointer data) {
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
	} else if (g_strcmp0(label, "=") == 0) {
		(void) entry_buf;
	} else {
		 if (g_strcmp0(label, "x assignment") == 0) {
			str = "x := ";
		} else if (g_strcmp0(label, "f definition") == 0) {
			str = "f := ";
		} else {
			str = tmp(label, bufstr, buflen);
		}
		gtk_entry_buffer_insert_text(entry_buf, buflen, str, strlen(str));
	}
}

static void on_assign_clicked(GtkButton *btn, gpointer data) {
	GtkBuilder *build = GTK_BUILDER(data);
	GtkWidget *entry;
	GtkWidget *tv;
	GtkEntryBuffer *entry_buf;
	GtkTextBuffer *tv_buf;
	const char *entry_buf_str;
	const char *label;
	guint entry_buf_len;
	GtkTextIter tv_buf_iter;

	entry = GTK_WIDGET(gtk_builder_get_object(build, "ent"));
	entry_buf = gtk_entry_get_buffer(GTK_ENTRY(entry));
	entry_buf_str = gtk_entry_buffer_get_text(entry_buf);
	entry_buf_len = gtk_entry_buffer_get_length(entry_buf);

	tv = GTK_WIDGET(gtk_builder_get_object(build, "tv"));
	tv_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
	gtk_text_buffer_get_end_iter(tv_buf, &tv_buf_iter);
	gtk_text_buffer_insert(tv_buf, &tv_buf_iter, "> ", 2);
	gtk_text_buffer_get_end_iter(tv_buf, &tv_buf_iter);
	gtk_text_buffer_insert(tv_buf, &tv_buf_iter, entry_buf_str, entry_buf_len);
	gtk_text_buffer_get_end_iter(tv_buf, &tv_buf_iter);
	gtk_text_buffer_insert(tv_buf, &tv_buf_iter, "\n", 1);
	
	gtk_entry_buffer_delete_text(entry_buf, 0, entry_buf_len);

}

static void app_activate(GApplication *app) {
	GtkBuilder *build;
	GtkWidget *win;
	GtkWidget *btn;
	const char *button_id_array[] = {
		"clear_button",
		"lbracket_button",
		"rbracket_button",
		"button0",
		"button1",
		"button2",
		"button3",
		"button4",
		"button5",
		"button6",
		"button7",
		"button8",
		"button9",
		"dot_button",
		"plus_button",
		"minus_button",
		"mul_button",
		"div_button",
		"pow_button",
		"mod_button",
		"cos_button",
		"sin_button",
		"tan_button",
		"acos_button",
		"asin_button",
		"atan_button",
		"sqrt_button",
		"ln_button",
		"log_button",
		"x_button",
		"f_button",
		"x_assignment_button",
		"f_definition_button",
	};

	build = gtk_builder_new_from_file("test.ui");
	win = GTK_WIDGET(gtk_builder_get_object(build, "win"));
	gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));

	for (size_t i = 0; i < sizeof(button_id_array) / sizeof(const char *); ++i) {
		btn = GTK_WIDGET(gtk_builder_get_object(build, button_id_array[i]));
		g_signal_connect(btn, "clicked", G_CALLBACK(on_clicked), build);
	}
	btn = GTK_WIDGET(gtk_builder_get_object(build, "assign_button"));
	g_signal_connect(btn, "clicked", G_CALLBACK(on_assign_clicked), build);

	gtk_window_present(GTK_WINDOW(win));
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;

	app = gtk_application_new("com.github.Vojan-Najov.SmartCalc_v1-0",
                              G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return (status);
}
