#include <pebble.h>

#if defined(PBL_ROUND)
  #define Y_OFFSET 5
#else
  #define Y_OFFSET -6
#endif

static Window *s_main_window;

static TextLayer *s_time_layer_main;
static TextLayer *s_time_layer_nw, *s_time_layer_w, *s_time_layer_sw, *s_time_layer_s, 
                 *s_time_layer_se, *s_time_layer_e, *s_time_layer_ne, *s_time_layer_n;
static GFont s_time_font;

static BitmapLayer *s_bg_layer;
static GBitmap *s_bg_bitmap;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // put hours and minutes into buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                        "%H:%M" : "%I:%M", tick_time);
  // display it
  text_layer_set_text(s_time_layer_main, s_buffer);   
  text_layer_set_text(s_time_layer_nw, s_buffer);   
  text_layer_set_text(s_time_layer_w, s_buffer);   
  text_layer_set_text(s_time_layer_sw, s_buffer);   
  text_layer_set_text(s_time_layer_s, s_buffer);   
  text_layer_set_text(s_time_layer_se, s_buffer);   
  text_layer_set_text(s_time_layer_e, s_buffer);   
  text_layer_set_text(s_time_layer_ne, s_buffer);   
  text_layer_set_text(s_time_layer_n, s_buffer);    
}

static void time_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // time
  s_time_layer_main = text_layer_create(GRect(0, Y_OFFSET, bounds.size.w, 52));
  s_time_layer_nw = text_layer_create(GRect(-2, -2 + Y_OFFSET, bounds.size.w, 52));
  s_time_layer_w = text_layer_create(GRect(-2, 0 + Y_OFFSET, bounds.size.w, 52));
  s_time_layer_sw = text_layer_create(GRect(-2, 2 + Y_OFFSET, bounds.size.w, 52));
  s_time_layer_s = text_layer_create(GRect(0, 2 + Y_OFFSET, bounds.size.w, 52));
  s_time_layer_se = text_layer_create(GRect(2, 2 + Y_OFFSET, bounds.size.w, 52));
  s_time_layer_e = text_layer_create(GRect(2, 0 + Y_OFFSET, bounds.size.w, 52));
  s_time_layer_ne = text_layer_create(GRect(2, -2 + Y_OFFSET, bounds.size.w, 52));
  s_time_layer_n = text_layer_create(GRect(0, -2 + Y_OFFSET, bounds.size.w, 52));
  // create and load time font
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_48));
  // stylize the text
  text_layer_set_background_color(s_time_layer_main, GColorClear);
  text_layer_set_background_color(s_time_layer_nw, GColorClear);
  text_layer_set_background_color(s_time_layer_w, GColorClear);
  text_layer_set_background_color(s_time_layer_sw, GColorClear);
  text_layer_set_background_color(s_time_layer_s, GColorClear);
  text_layer_set_background_color(s_time_layer_se, GColorClear);
  text_layer_set_background_color(s_time_layer_e, GColorClear);
  text_layer_set_background_color(s_time_layer_ne, GColorClear);
  text_layer_set_background_color(s_time_layer_n, GColorClear);
  text_layer_set_text_color(s_time_layer_main, GColorWhite);
  // text_layer_set_text_color(s_time_layer_main, GColorBlack);
  text_layer_set_text_color(s_time_layer_nw, GColorBlack);
  text_layer_set_text_color(s_time_layer_w, GColorBlack);
  text_layer_set_text_color(s_time_layer_sw, GColorBlack);
  text_layer_set_text_color(s_time_layer_s, GColorBlack);
  text_layer_set_text_color(s_time_layer_se, GColorBlack);
  text_layer_set_text_color(s_time_layer_e, GColorBlack);
  text_layer_set_text_color(s_time_layer_ne, GColorBlack);
  text_layer_set_text_color(s_time_layer_n, GColorBlack);
  text_layer_set_font(s_time_layer_main, s_time_font);
  text_layer_set_font(s_time_layer_nw, s_time_font);
  text_layer_set_font(s_time_layer_w, s_time_font);
  text_layer_set_font(s_time_layer_sw, s_time_font);
  text_layer_set_font(s_time_layer_s, s_time_font);
  text_layer_set_font(s_time_layer_se, s_time_font);
  text_layer_set_font(s_time_layer_e, s_time_font);
  text_layer_set_font(s_time_layer_ne, s_time_font);
  text_layer_set_font(s_time_layer_n, s_time_font);
  text_layer_set_text_alignment(s_time_layer_main, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_nw, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_w, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_sw, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_s, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_se, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_e, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_ne, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_n, GTextAlignmentCenter);

  // bitmaps
  // bg
  s_bg_layer = bitmap_layer_create(GRect((bounds.size.w - 180) / 2, (bounds.size.h - 180) / 2, 180, 180));
  // bitmap_layer_set_compositing_mode(s_gura_layer, GCompOpSet);
  s_bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG);
  bitmap_layer_set_bitmap(s_bg_layer, s_bg_bitmap);

  // stack up the layers
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bg_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_nw));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_w));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_sw));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_s));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_se));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_e));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_ne));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_n));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer_main));
}

static void main_window_unload(Window *window) {
  // unload text layers
  text_layer_destroy(s_time_layer_main);
  text_layer_destroy(s_time_layer_nw);
  text_layer_destroy(s_time_layer_w);
  text_layer_destroy(s_time_layer_sw);
  text_layer_destroy(s_time_layer_s);
  text_layer_destroy(s_time_layer_se);
  text_layer_destroy(s_time_layer_e);
  text_layer_destroy(s_time_layer_ne);
  text_layer_destroy(s_time_layer_n);

  // unload custom fonts
  fonts_unload_custom_font(s_time_font);
  
  // unload bitmap layers
  bitmap_layer_destroy(s_bg_layer);
  
  // unload gbitmaps
  gbitmap_destroy(s_bg_bitmap);
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