#include <pebble.h>

Window *main_window, *empty_window;
static TextLayer *s_time_layer, *s_date_layer, *s_day_layer;
static GFont time_font, date_font;
char s_buffer[] = "00:00";
char date_buffer[] = "December 31";
char day_buffer[] = "Wednesday AM";


static void hide_elements() {
  layer_set_hidden(text_layer_get_layer(s_time_layer), true);
  layer_set_hidden(text_layer_get_layer(s_date_layer), true);
  layer_set_hidden(text_layer_get_layer(s_day_layer), true);
}

static void show_elements() {
  layer_set_hidden(text_layer_get_layer(s_time_layer), false);
  layer_set_hidden(text_layer_get_layer(s_date_layer), false);
  layer_set_hidden(text_layer_get_layer(s_day_layer), false);
}


static void update_time() {

  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          // "%H:%M" : "%I:%M", tick_time);
                                          "%k:%M" : "%l:%M", tick_time);
  strftime(date_buffer, sizeof(date_buffer), "%B %d", tick_time);

  strftime(day_buffer, sizeof(day_buffer), clock_is_24h_style() ?"%A" : "%A", tick_time);
  
  if (clock_is_24h_style()) {
  	// text_layer_set_text(s_time_layer, "23:59");
  	text_layer_set_text(s_time_layer, s_buffer+((' ' == s_buffer[0])?1:0));
  } else {
  	// text_layer_set_text(s_time_layer, "23:59");
  	text_layer_set_text(s_time_layer, s_buffer+((' ' == s_buffer[0])?1:0));
  }
  text_layer_set_text(s_date_layer, date_buffer);
  text_layer_set_text(s_day_layer, day_buffer);
  show_elements();
}

static void tick_handler(struct tm *tick_time, TimeUnits changed) {
  // time_t temp = time(NULL);
  // struct tm *tick_time = localtime(&temp);

  if (MINUTE_UNIT & changed) {
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                            // "%H:%M" : "%I:%M", tick_time);
                                            "%k:%M" : "%l:%M", tick_time);
    if (clock_is_24h_style()) {
      // text_layer_set_text(s_time_layer, "23:59");
      text_layer_set_text(s_time_layer, s_buffer+((' ' == s_buffer[0])?1:0));
    } else {
      // text_layer_set_text(s_time_layer, "23:59");
      text_layer_set_text(s_time_layer, s_buffer+((' ' == s_buffer[0])?1:0));
    }
  }


  if (DAY_UNIT & changed) {
    strftime(date_buffer, sizeof(date_buffer), "%B %d", tick_time);
    strftime(day_buffer, sizeof(day_buffer), clock_is_24h_style() ?"%A" : "%A", tick_time);
    text_layer_set_text(s_date_layer, date_buffer);
    text_layer_set_text(s_day_layer, day_buffer);  
  }
}

static void main_window_load(Window *window) {
  // Get information about the Window
  window_set_background_color(main_window, GColorBlack);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  time_font = fonts_load_custom_font(
                        PBL_IF_ROUND_ELSE(
                        	resource_get_handle(RESOURCE_ID_MFONT_68),
                        	resource_get_handle(RESOURCE_ID_MFONT_62)
                        ));
  
  date_font = fonts_load_custom_font(
                        PBL_IF_ROUND_ELSE(
                        	resource_get_handle(RESOURCE_ID_MFONT_22),
                        	resource_get_handle(RESOURCE_ID_MFONT_20)
                        ));

  s_time_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(42, 42), 
                PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 72));
  s_date_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(120, 112), 
                PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 26));
  s_day_layer = text_layer_create(
        GRect(PBL_IF_ROUND_ELSE(1, 0), PBL_IF_ROUND_ELSE(25, 28), 
                PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 26));

  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
//   text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, PBL_IF_COLOR_ELSE(GColorWhite, GColorWhite));
//   text_layer_set_text(s_date_layer, "Sun Apr 31");
  text_layer_set_font(s_date_layer, date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  text_layer_set_background_color(s_day_layer, GColorClear);
  text_layer_set_text_color(s_day_layer, PBL_IF_COLOR_ELSE(GColorWhite, GColorWhite));
//   text_layer_set_text(s_date_layer, "Sun Apr 31");
  text_layer_set_font(s_day_layer, date_font);
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);


  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_day_layer));

  hide_elements();
  update_time();
}

static void main_window_unload(Window *window) {
  hide_elements();
}


static void switch_to_empty() {
  window_stack_push(empty_window, false);
}

static void show_time() {
  window_stack_push(main_window, false);
}

void load_agents() {
  update_time();
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler); 
  layer_set_hidden(text_layer_get_layer(s_time_layer), false);
  layer_set_hidden(text_layer_get_layer(s_date_layer), false);
  layer_set_hidden(text_layer_get_layer(s_day_layer), false);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Switching back to empty in 8 seconds");
  AppTimer *updateTimer = app_timer_register(8000, 
          (AppTimerCallback) switch_to_empty, NULL);
}

void unload_agents() {
  tick_timer_service_unsubscribe();
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  show_time();
}


void handle_init(void) {
  main_window = window_create();
  empty_window = window_create();
  
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .appear = load_agents,
    .disappear = unload_agents,
    .unload = main_window_unload
  });

  window_set_window_handlers(empty_window, (WindowHandlers) {
    .load = 0,
    .unload = 0,
  });

  window_set_background_color(empty_window, GColorBlack);
  switch_to_empty();
  accel_tap_service_subscribe(accel_tap_handler);
}

void handle_deinit(void) {
  accel_tap_service_unsubscribe();
  window_destroy(main_window);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_day_layer);
  fonts_unload_custom_font(time_font);
  fonts_unload_custom_font(date_font);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
