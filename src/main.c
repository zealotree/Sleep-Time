#include <pebble.h>

Window *main_window, *empty_window;
static TextLayer *s_time_layer, *s_date_layer;
static GFont time_font, date_font;
char s_buffer[] = "00:00";
char date_buffer[] = "SUN APR 31";

static void update_time() {

  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H%M" : "%I%M", tick_time);
  strftime(date_buffer, sizeof(date_buffer), "%a %b %d", tick_time);
  
  text_layer_set_text(s_time_layer, s_buffer);  
  text_layer_set_text(s_date_layer, date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  window_set_background_color(main_window, GColorBlack);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  time_font = fonts_load_custom_font(
                        resource_get_handle(RESOURCE_ID_MFONT_46));
  
  date_font = fonts_load_custom_font(
                        resource_get_handle(RESOURCE_ID_MFONT_18));

  s_time_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(48, 44), 
                PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 50));
  s_date_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(95, 89), 
                PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 50));

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

  update_time();

}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  fonts_unload_custom_font(time_font);
  fonts_unload_custom_font(date_font);
}



static void main_window_unload_agents() {
  tick_timer_service_unsubscribe();
}

static void switch_to_main_window() {
  window_stack_push(main_window, false);
}

static void main_window_load_agents() {
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
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
  
  switch_to_main_window();
}

void handle_deinit(void) {
  window_destroy(main_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
