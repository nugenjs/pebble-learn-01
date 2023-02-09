#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;
static TextLayer *s_time_layer;
static GFont s_time_font;

static TextLayer *s_weather_layer;
static GFont s_weather_font;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[15];
  strftime(s_buffer, sizeof(s_buffer), 
    clock_is_24h_style() ? "%I%M%S" : "%H:%M:%S", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Background stoof
  window_set_background_color(window, GColorBlue);
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  // TIME LAYER
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50)); // Create the TextLayer with specific bounds
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlue);
  // text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_32)); // Custom font
  text_layer_set_font(s_time_layer, s_time_font);
  // text_layer_set_text(s_time_layer, "04:20");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  // WEATHER LAYER
  s_weather_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(125, 120), bounds.size.w, 25)); 
  text_layer_set_background_color(s_weather_layer, GColorGreen);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20)); // Custom font
  text_layer_set_font(s_weather_layer, s_weather_font);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_text(s_weather_layer, "Loading...");
  layer_add_child(window_layer, text_layer_get_layer(s_weather_layer));
}
static void main_window_unload(Window *window) {
  bitmap_layer_destroy(s_background_layer);
  gbitmap_destroy(s_background_bitmap);


  // Destroy TextLayer
  fonts_unload_custom_font(s_time_font);
  text_layer_destroy(s_time_layer);

  fonts_unload_custom_font(s_weather_font);
  text_layer_destroy(s_weather_layer);
}
static void init () {
  s_window = window_create();

  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_window, true);

  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

  // Make sure the time is displayed from the start
  update_time();

  printf("init end");
}
static void deinit () {
  window_destroy(s_window);

  tick_timer_service_unsubscribe();
}
int main(void) {
  init();
  // APP_LOG_LEVEL_INFO('asdf');
  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "main app_log 01");
  printf("main 01");
;  app_event_loop();
  printf("main 02");
  deinit();
}
