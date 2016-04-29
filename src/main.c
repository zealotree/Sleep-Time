#include <pebble.h>
#include <ctype.h>

Window *main_window, *empty_window;
static TextLayer *s_time_layer, *s_date_layer;
static GFont time_font, date_font;
char s_buffer[] = "00:00 PM";
char date_buffer[] = "SUN APR 31";

static void tick_handler(struct tm *tick_time, TimeUnits changed) {
  
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M %p", tick_time);
  strftime(date_buffer, sizeof(date_buffer), "%a %b %d", tick_time);
  
  text_layer_set_text(s_time_layer, s_buffer);  
  text_layer_set_text(s_date_layer, date_buffer);
}

static void main_window_load(Window *window) {
  // Get information about the Window
  window_set_background_color(main_window, GColorBlack);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  if (clock_is_24h_style()){
    time_font = fonts_load_custom_font(
                        resource_get_handle(RESOURCE_ID_MFONT_38));
  } else {
    time_font = fonts_load_custom_font(
                        resource_get_handle(RESOURCE_ID_MFONT_28));
  }
  
  date_font = fonts_load_custom_font(
                        resource_get_handle(RESOURCE_ID_MFONT_18));

  if (clock_is_24h_style()){
    s_time_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(52, 44), 
              PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 50));
    s_date_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(90, 84), 
              PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 50));
  } else {
    s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(61, 53), 
            PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 50));
    s_date_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(89, 87), 
            PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 50));
  }

  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
//   text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, PBL_IF_COLOR_ELSE(GColorDarkGray, GColorWhite));
//   text_layer_set_text(s_date_layer, "Sun Apr 31");
  text_layer_set_font(s_date_layer, date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  
}

static void main_window_unload(Window *window) {
  window_destroy(main_window);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  fonts_unload_custom_font(time_font);
  fonts_unload_custom_font(date_font);
}



static void main_window_unload_agents() {
  tick_timer_service_unsubscribe();
}

static void switch_to_empty_window() {
  window_stack_push(empty_window, false);
}

static void switch_to_main_window() {
  window_stack_push(main_window, false);
}

static void main_window_load_agents() {
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Switching back to empty in 10 seconds");
  AppTimer *updateTimer = app_timer_register(10000, 
          (AppTimerCallback) switch_to_empty_window, NULL);
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  switch_to_main_window();
}

void handle_init(void) {
  main_window = window_create();
  
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .appear = main_window_load_agents,
    .disappear = main_window_unload_agents,
    .unload = main_window_unload
  });
  
  empty_window = window_create();

  window_set_window_handlers(empty_window, (WindowHandlers) {
    .load = 0,
    .unload = 0,
  });
  
  window_set_background_color(empty_window, GColorBlack);
  
  switch_to_empty_window();
  accel_tap_service_subscribe(accel_tap_handler);
}

void handle_deinit(void) {
  accel_tap_service_unsubscribe();
  window_destroy(empty_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
