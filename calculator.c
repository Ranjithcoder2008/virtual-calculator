#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *display;

double first_number = 0;
char operation = 0;
int new_number = 1;


/* =====================================================
   NUMBER BUTTON
   ===================================================== */

static void number_clicked(GtkWidget *button, gpointer data)
{
    const char *value =
        gtk_button_get_label(GTK_BUTTON(button));

    const char *current =
        gtk_editable_get_text(GTK_EDITABLE(display));

    if (strcmp(value, ".") == 0)
    {
        if (new_number)
        {
            gtk_editable_set_text(
                GTK_EDITABLE(display), "0."
            );

            new_number = 0;
            return;
        }

        if (strchr(current, '.') != NULL)
            return;
    }

    if (new_number || strcmp(current, "0") == 0)
    {
        gtk_editable_set_text(
            GTK_EDITABLE(display), value
        );

        new_number = 0;
    }
    else
    {
        char result[100];

        snprintf(
            result,
            sizeof(result),
            "%s%s",
            current,
            value
        );

        gtk_editable_set_text(
            GTK_EDITABLE(display),
            result
        );
    }
}


/* =====================================================
   OPERATOR
   ===================================================== */

static void operator_clicked(GtkWidget *button, gpointer data)
{
    const char *op =
        gtk_button_get_label(GTK_BUTTON(button));

    const char *text =
        gtk_editable_get_text(GTK_EDITABLE(display));

    first_number = atof(text);

    if (strcmp(op, "×") == 0)
        operation = '*';
    else if (strcmp(op, "÷") == 0)
        operation = '/';
    else
        operation = op[0];

    new_number = 1;
}


/* =====================================================
   EQUALS
   ===================================================== */

static void equals_clicked(GtkWidget *button, gpointer data)
{
    const char *text =
        gtk_editable_get_text(GTK_EDITABLE(display));

    double second_number = atof(text);
    double result = 0;

    switch (operation)
    {
        case '+':
            result = first_number + second_number;
            break;

        case '-':
            result = first_number - second_number;
            break;

        case '*':
            result = first_number * second_number;
            break;

        case '/':
            if (second_number == 0)
            {
                gtk_editable_set_text(
                    GTK_EDITABLE(display),
                    "Error"
                );

                new_number = 1;
                return;
            }

            result = first_number / second_number;
            break;

        default:
            result = second_number;
    }

    char answer[100];

    snprintf(
        answer,
        sizeof(answer),
        "%.10g",
        result
    );

    gtk_editable_set_text(
        GTK_EDITABLE(display),
        answer
    );

    new_number = 1;
}


/* =====================================================
   CLEAR
   ===================================================== */

static void clear_clicked(GtkWidget *button, gpointer data)
{
    gtk_editable_set_text(
        GTK_EDITABLE(display),
        "0"
    );

    first_number = 0;
    operation = 0;
    new_number = 1;
}


/* =====================================================
   BACKSPACE
   ===================================================== */

static void backspace_clicked(GtkWidget *button, gpointer data)
{
    const char *current =
        gtk_editable_get_text(GTK_EDITABLE(display));

    int length = strlen(current);

    if (length <= 1 || strcmp(current, "Error") == 0)
    {
        gtk_editable_set_text(
            GTK_EDITABLE(display),
            "0"
        );

        return;
    }

    char result[100];

    strncpy(result, current, length - 1);

    result[length - 1] = '\0';

    gtk_editable_set_text(
        GTK_EDITABLE(display),
        result
    );
}


/* =====================================================
   PERCENTAGE
   ===================================================== */

static void percent_clicked(GtkWidget *button, gpointer data)
{
    const char *current =
        gtk_editable_get_text(GTK_EDITABLE(display));

    double number = atof(current);

    number = number / 100.0;

    char result[100];

    snprintf(
        result,
        sizeof(result),
        "%.10g",
        number
    );

    gtk_editable_set_text(
        GTK_EDITABLE(display),
        result
    );
}


/* =====================================================
   PLUS / MINUS
   ===================================================== */

static void sign_clicked(GtkWidget *button, gpointer data)
{
    const char *current =
        gtk_editable_get_text(GTK_EDITABLE(display));

    double number = atof(current);

    number = -number;

    char result[100];

    snprintf(
        result,
        sizeof(result),
        "%.10g",
        number
    );

    gtk_editable_set_text(
        GTK_EDITABLE(display),
        result
    );
}


/* =====================================================
   CREATE BUTTON
   ===================================================== */

static GtkWidget *create_button(
    const char *text,
    GtkGrid *grid,
    int column,
    int row,
    const char *css_class)
{
    GtkWidget *button;

    button = gtk_button_new_with_label(text);

    gtk_widget_add_css_class(
        button,
        css_class
    );

    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);

    gtk_grid_attach(
        grid,
        button,
        column,
        row,
        1,
        1
    );

    return button;
}


/* =====================================================
   KEYBOARD SUPPORT
   ===================================================== */

static gboolean key_pressed(
    GtkEventControllerKey *controller,
    guint keyval,
    guint keycode,
    GdkModifierType state,
    gpointer user_data)
{
    /* Numbers 0-9 */

    if (keyval >= GDK_KEY_0 &&
        keyval <= GDK_KEY_9)
    {
        char number[2];

        number[0] = (char)keyval;
        number[1] = '\0';

        const char *current =
            gtk_editable_get_text(
                GTK_EDITABLE(display)
            );

        if (new_number || strcmp(current, "0") == 0)
        {
            gtk_editable_set_text(
                GTK_EDITABLE(display),
                number
            );

            new_number = 0;
        }
        else
        {
            char result[100];

            snprintf(
                result,
                sizeof(result),
                "%s%s",
                current,
                number
            );

            gtk_editable_set_text(
                GTK_EDITABLE(display),
                result
            );
        }

        return TRUE;
    }


    /* Decimal */

    if (keyval == GDK_KEY_period)
    {
        const char *current =
            gtk_editable_get_text(
                GTK_EDITABLE(display)
            );

        if (strchr(current, '.') != NULL)
            return TRUE;

        if (new_number)
        {
            gtk_editable_set_text(
                GTK_EDITABLE(display),
                "0."
            );

            new_number = 0;
        }
        else
        {
            char result[100];

            snprintf(
                result,
                sizeof(result),
                "%s.",
                current
            );

            gtk_editable_set_text(
                GTK_EDITABLE(display),
                result
            );
        }

        return TRUE;
    }


    /* Addition */

    if (keyval == GDK_KEY_plus)
    {
        first_number =
            atof(
                gtk_editable_get_text(
                    GTK_EDITABLE(display)
                )
            );

        operation = '+';
        new_number = 1;

        return TRUE;
    }


    /* Subtraction */

    if (keyval == GDK_KEY_minus)
    {
        first_number =
            atof(
                gtk_editable_get_text(
                    GTK_EDITABLE(display)
                )
            );

        operation = '-';
        new_number = 1;

        return TRUE;
    }


    /* Multiplication */

    if (keyval == GDK_KEY_asterisk)
    {
        first_number =
            atof(
                gtk_editable_get_text(
                    GTK_EDITABLE(display)
                )
            );

        operation = '*';
        new_number = 1;

        return TRUE;
    }


    /* Division */

    if (keyval == GDK_KEY_slash)
    {
        first_number =
            atof(
                gtk_editable_get_text(
                    GTK_EDITABLE(display)
                )
            );

        operation = '/';
        new_number = 1;

        return TRUE;
    }


    /* Enter */

    if (keyval == GDK_KEY_Return ||
        keyval == GDK_KEY_KP_Enter)
    {
        equals_clicked(NULL, NULL);

        return TRUE;
    }


    /* Backspace */

    if (keyval == GDK_KEY_BackSpace)
    {
        backspace_clicked(NULL, NULL);

        return TRUE;
    }


    /* Escape */

    if (keyval == GDK_KEY_Escape)
    {
        clear_clicked(NULL, NULL);

        return TRUE;
    }


    /* Percentage */

    if (keyval == GDK_KEY_percent)
    {
        percent_clicked(NULL, NULL);

        return TRUE;
    }

    return FALSE;
}


/* =====================================================
   MAIN WINDOW
   ===================================================== */

static void activate(
    GtkApplication *app,
    gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkCssProvider *css_provider;
    GtkEventController *key_controller;


    /* CSS */

    css_provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path(
        css_provider,
        "style.css"
    );

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    g_object_unref(css_provider);


    /* WINDOW */

    window =
        gtk_application_window_new(app);

    gtk_window_set_title(
        GTK_WINDOW(window),
        "Virtual Calculator"
    );

    gtk_window_set_default_size(
        GTK_WINDOW(window),
        450,
        700
    );

    gtk_window_set_resizable(
        GTK_WINDOW(window),
        FALSE
    );


    /* KEYBOARD */

    key_controller =
        GTK_EVENT_CONTROLLER(
            gtk_event_controller_key_new()
        );

    g_signal_connect(
        key_controller,
        "key-pressed",
        G_CALLBACK(key_pressed),
        NULL
    );

    gtk_widget_add_controller(
        window,
        key_controller
    );


    /* GRID */

    grid = gtk_grid_new();

    gtk_grid_set_row_spacing(
        GTK_GRID(grid),
        8
    );

    gtk_grid_set_column_spacing(
        GTK_GRID(grid),
        8
    );

    gtk_widget_set_margin_top(grid, 15);
    gtk_widget_set_margin_bottom(grid, 15);
    gtk_widget_set_margin_start(grid, 15);
    gtk_widget_set_margin_end(grid, 15);


    /* DISPLAY */

    display = gtk_entry_new();

    gtk_editable_set_text(
        GTK_EDITABLE(display),
        "0"
    );

    gtk_editable_set_editable(
        GTK_EDITABLE(display),
        FALSE
    );

    gtk_widget_set_size_request(
        display,
        -1,
        100
    );

    gtk_widget_set_hexpand(
        display,
        TRUE
    );

    gtk_grid_attach(
        GTK_GRID(grid),
        display,
        0,
        0,
        4,
        1
    );


    /* ROW 1 */

    GtkWidget *clear =
        create_button(
            "AC", GTK_GRID(grid),
            0, 1, "clear"
        );

    GtkWidget *backspace =
        create_button(
            "⌫", GTK_GRID(grid),
            1, 1, "function"
        );

    GtkWidget *percent =
        create_button(
            "%", GTK_GRID(grid),
            2, 1, "function"
        );

    GtkWidget *divide =
        create_button(
            "÷", GTK_GRID(grid),
            3, 1, "operator"
        );


    /* ROW 2 */

    GtkWidget *b7 =
        create_button(
            "7", GTK_GRID(grid),
            0, 2, "number"
        );

    GtkWidget *b8 =
        create_button(
            "8", GTK_GRID(grid),
            1, 2, "number"
        );

    GtkWidget *b9 =
        create_button(
            "9", GTK_GRID(grid),
            2, 2, "number"
        );

    GtkWidget *multiply =
        create_button(
            "×", GTK_GRID(grid),
            3, 2, "operator"
        );


    /* ROW 3 */

    GtkWidget *b4 =
        create_button(
            "4", GTK_GRID(grid),
            0, 3, "number"
        );

    GtkWidget *b5 =
        create_button(
            "5", GTK_GRID(grid),
            1, 3, "number"
        );

    GtkWidget *b6 =
        create_button(
            "6", GTK_GRID(grid),
            2, 3, "number"
        );

    GtkWidget *subtract =
        create_button(
            "-", GTK_GRID(grid),
            3, 3, "operator"
        );


    /* ROW 4 */

    GtkWidget *b1 =
        create_button(
            "1", GTK_GRID(grid),
            0, 4, "number"
        );

    GtkWidget *b2 =
        create_button(
            "2", GTK_GRID(grid),
            1, 4, "number"
        );

    GtkWidget *b3 =
        create_button(
            "3", GTK_GRID(grid),
            2, 4, "number"
        );

    GtkWidget *add =
        create_button(
            "+", GTK_GRID(grid),
            3, 4, "operator"
        );


    /* ROW 5 */

    GtkWidget *sign =
        create_button(
            "±", GTK_GRID(grid),
            0, 5, "function"
        );

    GtkWidget *b0 =
        create_button(
            "0", GTK_GRID(grid),
            1, 5, "number"
        );

    GtkWidget *decimal =
        create_button(
            ".", GTK_GRID(grid),
            2, 5, "number"
        );

    GtkWidget *equals =
        create_button(
            "=", GTK_GRID(grid),
            3, 5, "equals"
        );


    /* NUMBER SIGNALS */

    g_signal_connect(b0, "clicked",
        G_CALLBACK(number_clicked), NULL);

    g_signal_connect(b1, "clicked",
        G_CALLBACK(number_clicked), NULL);

    g_signal_connect(b2, "clicked",
        G_CALLBACK(number_clicked), NULL);

    g_signal_connect(b3, "clicked",
        G_CALLBACK(number_clicked), NULL);

    g_signal_connect(b4, "clicked",
        G_CALLBACK(number_clicked), NULL);

    g_signal_connect(b5, "clicked",
        G_CALLBACK(number_clicked), NULL);

    g_signal_connect(b6, "clicked",
        G_CALLBACK(number_clicked), NULL);

    g_signal_connect(b7, "clicked",
        G_CALLBACK(number_clicked), NULL);

    g_signal_connect(b8, "clicked",
        G_CALLBACK(number_clicked), NULL);

    g_signal_connect(b9, "clicked",
        G_CALLBACK(number_clicked), NULL);

    g_signal_connect(decimal, "clicked",
        G_CALLBACK(number_clicked), NULL);


    /* OPERATOR SIGNALS */

    g_signal_connect(add, "clicked",
        G_CALLBACK(operator_clicked), NULL);

    g_signal_connect(subtract, "clicked",
        G_CALLBACK(operator_clicked), NULL);

    g_signal_connect(multiply, "clicked",
        G_CALLBACK(operator_clicked), NULL);

    g_signal_connect(divide, "clicked",
        G_CALLBACK(operator_clicked), NULL);


    /* OTHER SIGNALS */

    g_signal_connect(clear, "clicked",
        G_CALLBACK(clear_clicked), NULL);

    g_signal_connect(backspace, "clicked",
        G_CALLBACK(backspace_clicked), NULL);

    g_signal_connect(percent, "clicked",
        G_CALLBACK(percent_clicked), NULL);

    g_signal_connect(sign, "clicked",
        G_CALLBACK(sign_clicked), NULL);

    g_signal_connect(equals, "clicked",
        G_CALLBACK(equals_clicked), NULL);


    /* SHOW */

    gtk_window_set_child(
        GTK_WINDOW(window),
        grid
    );

    gtk_window_present(
        GTK_WINDOW(window)
    );
}


/* =====================================================
   MAIN
   ===================================================== */

int main(int argc, char *argv[])
{
    GtkApplication *app;
    int status;

    app =
        gtk_application_new(
            "com.example.virtualcalculator",
            G_APPLICATION_DEFAULT_FLAGS
        );

    g_signal_connect(
        app,
        "activate",
        G_CALLBACK(activate),
        NULL
    );

    status =
        g_application_run(
            G_APPLICATION(app),
            argc,
            argv
        );

    g_object_unref(app);

    return status;
}