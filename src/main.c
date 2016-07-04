#include <pebble.h>

Window *main_window, *empty_window;
#if defined(PBL_ROUND)
static Layer *s_canvas_layer;
#endif
static TextLayer *s_time_layer, *s_date_layer, *s_day_layer, *ap_layer;
static GFont time_font, date_font, extra_font;
char s_buffer[] = "00:00";
char date_buffer[] = "December 31";
char day_buffer[] = "Wednesday AM";
char a_buffer[] = "W53 D365";

static void update_time() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                            // "%H:%M" : "%I:%M", tick_time);
                                            "%k:%M" : "%l:%M", tick_time);
    if (clock_is_24h_style()) {
      // text_layer_set_text(s_time_layer, "23:59");
      text_layer_set_text(s_time_layer, s_buffer+((' ' == s_buffer[0])?1:0));
      text_layer_set_font(ap_layer, extra_font);
      strftime(a_buffer, sizeof(a_buffer), "W%V D%j", tick_time);
      text_layer_set_text(ap_layer, a_buffer);
      layer_set_hidden(text_layer_get_layer(ap_layer), false);
    } else {
      // text_layer_set_text(s_time_layer, "23:59");
      text_layer_set_text(s_time_layer, s_buffer+((' ' == s_buffer[0])?1:0));
      text_layer_set_font(ap_layer, date_font);
      strftime(a_buffer, sizeof(a_buffer), "%p", tick_time);
      text_layer_set_text(ap_layer, a_buffer);
      layer_set_hidden(text_layer_get_layer(ap_layer), false);
      // text_layer_set_font(ap_layer, date_font);
      // strftime(a_buffer, sizeof(a_buffer), "%p", tick_time);
      // text_layer_set_text(ap_layer, a_buffer);
      // layer_set_hidden(text_layer_get_layer(ap_layer), false);
    }
    layer_set_hidden(text_layer_get_layer(s_time_layer), false);

    strftime(date_buffer, sizeof(date_buffer), "%B %d", tick_time);
    strftime(day_buffer, sizeof(day_buffer), clock_is_24h_style() ?"%A" : "%A", tick_time);
    text_layer_set_text(s_date_layer, date_buffer);
    text_layer_set_text(s_day_layer, day_buffer);  
    layer_set_hidden(text_layer_get_layer(s_date_layer), false);
    layer_set_hidden(text_layer_get_layer(s_day_layer), false);
}

static void tick_handler(struct tm *tick_time, TimeUnits changed) {
  // time_t temp = time(NULL);
  // struct tm *tick_time = localtime(&temp);

  if (MINUTE_UNIT) {
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                            // "%H:%M" : "%I:%M", tick_time);
                                            "%k:%M" : "%l:%M", tick_time);

    if (clock_is_24h_style()) {
      // text_layer_set_text(s_time_layer, "23:59");
      text_layer_set_text(s_time_layer, s_buffer+((' ' == s_buffer[0])?1:0));
      text_layer_set_font(ap_layer, extra_font);
      strftime(a_buffer, sizeof(a_buffer), "W%V D%j", tick_time);
      text_layer_set_text(ap_layer, a_buffer);
      layer_set_hidden(text_layer_get_layer(ap_layer), false);
    } else {
      // text_layer_set_text(s_time_layer, "23:59");
      text_layer_set_text(s_time_layer, s_buffer+((' ' == s_buffer[0])?1:0));
      text_layer_set_font(ap_layer, date_font);
      strftime(a_buffer, sizeof(a_buffer), "%p", tick_time);
      text_layer_set_text(ap_layer, a_buffer);
      layer_set_hidden(text_layer_get_layer(ap_layer), false);
      // text_layer_set_font(ap_layer, date_font);
      // strftime(a_buffer, sizeof(a_buffer), "%p", tick_time);
      // text_layer_set_text(ap_layer, a_buffer);
      // layer_set_hidden(text_layer_get_layer(ap_layer), false);
    }
    layer_set_hidden(text_layer_get_layer(s_time_layer), false);
  }


  if (DAY_UNIT) {
    strftime(date_buffer, sizeof(date_buffer), "%B %d", tick_time);
    strftime(day_buffer, sizeof(day_buffer), clock_is_24h_style() ?"%A" : "%A", tick_time);
    text_layer_set_text(s_date_layer, date_buffer);
    text_layer_set_text(s_day_layer, day_buffer);  
    layer_set_hidden(text_layer_get_layer(s_date_layer), false);
    layer_set_hidden(text_layer_get_layer(s_day_layer), false);
  }
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!
  GRect rect_bounds = layer_get_bounds(layer);
  int32_t angle_start = DEG_TO_TRIGANGLE(0);
  int32_t full = DEG_TO_TRIGANGLE(360);

  graphics_context_set_stroke_color(ctx, GColorLiberty);
  graphics_context_set_fill_color(ctx, GColorLiberty);

  graphics_fill_radial(ctx, rect_bounds, GOvalScaleModeFitCircle, 7, angle_start, 
                                                                    full);

  BatteryChargeState state = battery_state_service_peek();

  if (state.charge_percent <= 20) {
    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));    
    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));    
  } else if (state.charge_percent > 20 && state.charge_percent <= 40) {
    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorMelon, GColorWhite));    
    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorMelon, GColorWhite));    
  } else if (state.charge_percent > 40 && state.charge_percent <= 50) {
    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));    
    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
  } else if (state.charge_percent > 50 && state.charge_percent <= 100) {
    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorMediumAquamarine, GColorWhite));    
    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorMediumAquamarine, GColorWhite));    
  }


  int32_t angle_end = DEG_TO_TRIGANGLE((360/10 * state.charge_percent)/10);
  graphics_fill_radial(ctx, rect_bounds, GOvalScaleModeFitCircle, 7, angle_start, 
                                                                    angle_end);



}


static void main_window_load(Window *window) {
  // Get information about the Window
  window_set_background_color(main_window, PBL_IF_COLOR_ELSE(GColorBlack, GColorBlack));
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  #if defined(PBL_ROUND)
  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_get_root_layer(window), s_canvas_layer);
  #endif
  
  time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MFONT_64));
  
  date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MFONT_22));

  extra_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MFONT_18));

  s_time_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(38, 42), 
                PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 72));
  s_date_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(107, 111), 
                PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 26));
  s_day_layer = text_layer_create(
        GRect(PBL_IF_ROUND_ELSE(1, 0), PBL_IF_ROUND_ELSE(24, 28), 
                PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 26));

  ap_layer = text_layer_create(
        GRect(PBL_IF_ROUND_ELSE(0, 0), PBL_IF_ROUND_ELSE(133, 120), 
                PBL_IF_ROUND_ELSE(bounds.size.w, bounds.size.w), 26));

  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_font(s_time_layer, time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_color(s_time_layer, PBL_IF_COLOR_ELSE(GColorWhite, GColorWhite));
  
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

  text_layer_set_background_color(ap_layer, GColorClear);
  text_layer_set_text_color(ap_layer, PBL_IF_COLOR_ELSE(GColorWhite, GColorWhite));
  text_layer_set_font(ap_layer, date_font);
  text_layer_set_text_alignment(ap_layer, GTextAlignmentCenter);

  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_day_layer));
  layer_add_child(window_layer, text_layer_get_layer(ap_layer));

  layer_set_hidden(text_layer_get_layer(s_time_layer), true);
  layer_set_hidden(text_layer_get_layer(s_day_layer), true);
  layer_set_hidden(text_layer_get_layer(s_date_layer), true);
  layer_set_hidden(text_layer_get_layer(ap_layer), true);

  update_time();
}

static void hide_elements() {
  layer_set_hidden(text_layer_get_layer(s_time_layer), true);
  layer_set_hidden(text_layer_get_layer(s_date_layer), true);
  layer_set_hidden(text_layer_get_layer(s_day_layer), true);
  layer_set_hidden(text_layer_get_layer(ap_layer), true);
  text_layer_set_text(s_time_layer, "");
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
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler); 
  // light_enable_interaction();
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
  #if defined(PBL_ROUND)
  if (s_canvas_layer) {
    layer_destroy(s_canvas_layer);
  }
  #endif
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(ap_layer);
  fonts_unload_custom_font(time_font);
  fonts_unload_custom_font(date_font);
  fonts_unload_custom_font(extra_font);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
