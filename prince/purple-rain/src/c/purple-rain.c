#include <pebble.h>

#if defined(PBL_ROUND)
  #define Y_OFFSET 2
  #define X_OFFSET 66
  #define TEXT_WIDTH 90
#else
  #define Y_OFFSET -10
  #define X_OFFSET 66
  #define TEXT_WIDTH 72
#endif
#define TEXT_Y_OFFSET 42
#define TEXT_X_OFFSET 0

static Window *s_main_window;

static TextLayer *s_time_layer_hour, *s_time_layer_min;
static GFont s_time_font;

static BitmapLayer *s_prince_layer;
static GBitmap *s_prince_bitmap;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // put hours and minutes into buffer
  static char s_hour_buffer[4];
  strftime(s_hour_buffer, sizeof(s_hour_buffer), clock_is_24h_style() ?
                                        "%H" : "%I", tick_time);
  static char s_min_buffer[4];
  strftime(s_min_buffer, sizeof(s_min_buffer), "%M", tick_time);

  // display it
  text_layer_set_text(s_time_layer_hour, s_hour_buffer);   
  text_layer_set_text(s_time_layer_min, s_min_buffer);
}

static void time_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // time
  s_time_layer_hour = text_layer_create(GRect(X_OFFSET, Y_OFFSET, TEXT_WIDTH, 60));
  s_time_layer_min = text_layer_create(GRect(X_OFFSET, Y_OFFSET + TEXT_Y_OFFSET, TEXT_WIDTH, 60));
  // create and load time font
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_46));
  // stylize the text
  text_layer_set_background_color(s_time_layer_hour, GColorClear);
  text_layer_set_background_color(s_time_layer_min, GColorClear);
  text_layer_set_text_color(s_time_layer_hour, GColorBlack);
  text_layer_set_text_color(s_time_layer_min, GColorBlack);
  text_layer_set_font(s_time_layer_hour, s_time_font);
  text_layer_set_font(s_time_layer_min, s_time_font);
  text_layer_set_text_alignment(s_time_layer_hour, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_min, GTextAlignmentCenter);

  // bitmaps
  // bg
  s_prince_layer = bitmap_layer_create(GRect((bounds.size.w - 180) / 2, (bounds.size.h - 180) / 2, 180, 180));
  // bitmap_layer_set_compositing_mode(s_prince_layer, GCompOpSet);
  s_prince_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PRINCE);
  bitmap_layer_set_bitmap(s_prince_layer, s_prince_bitmap);

  // stack up the layers
  layer_add_child(window_layer, bitmap_layer_get_layer(s_prince_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_hour));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_min));
}

static void main_window_unload(Window *window) {
  // unload text layers
  text_layer_destroy(s_time_layer_hour);
  text_layer_destroy(s_time_layer_min);

  // unload custom fonts
  fonts_unload_custom_font(s_time_font);
  
  // unload bitmap layers
  bitmap_layer_destroy(s_prince_layer);
  
  // unload gbitmaps
  gbitmap_destroy(s_prince_bitmap);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // set up tick_handler to run every minute
  tick_timer_service_subscribe(MINUTE_UNIT, time_handler);
  // want to display time and at the start
  update_time();
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}