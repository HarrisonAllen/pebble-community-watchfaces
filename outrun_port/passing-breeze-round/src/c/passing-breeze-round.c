#include <pebble.h>

#define NUM_BGS 20
#define NUM_RDS 7
#define BG_HEIGHT 142
#define RD_HEIGHT 38
#define TIME_Y 5
#define DATE_Y 52
#define BATT_TEXT_X 30
#define BATT_TEXT_Y 140
#define BATT_ICON_X 20
#define BATT_ICON_Y 154
#define CAR_X 70
#define CAR_Y 154
#define DEMO_MODE false

static Window *s_main_window;

static TextLayer *s_time_layer, *s_date_layer, *s_battery_text_layer;
static GFont s_time_font, s_date_font, s_battery_font;

static BitmapLayer *s_background_layer, *s_road_layer, *s_battery_layer, *s_car_layer;
static GBitmap *s_background_bitmap, *s_road_bitmap, *s_battery_bitmap, *s_car_bitmap;


static const uint32_t BACKGROUNDS[] = {
  RESOURCE_ID_IMAGE_BG_00,
  RESOURCE_ID_IMAGE_BG_01,
  RESOURCE_ID_IMAGE_BG_02,
  RESOURCE_ID_IMAGE_BG_03,
  RESOURCE_ID_IMAGE_BG_04,
  RESOURCE_ID_IMAGE_BG_05,
  RESOURCE_ID_IMAGE_BG_06,
  RESOURCE_ID_IMAGE_BG_07,
  RESOURCE_ID_IMAGE_BG_08,
  RESOURCE_ID_IMAGE_BG_09,
  RESOURCE_ID_IMAGE_BG_10,
  RESOURCE_ID_IMAGE_BG_11,
  RESOURCE_ID_IMAGE_BG_12,
  RESOURCE_ID_IMAGE_BG_13,
  RESOURCE_ID_IMAGE_BG_14,
  RESOURCE_ID_IMAGE_BG_15,
  RESOURCE_ID_IMAGE_BG_16,
  RESOURCE_ID_IMAGE_BG_17,
  RESOURCE_ID_IMAGE_BG_18,
  RESOURCE_ID_IMAGE_BG_19,
};

static const uint32_t ROADS[] = {
  RESOURCE_ID_IMAGE_RD_0,
  RESOURCE_ID_IMAGE_RD_1,
  RESOURCE_ID_IMAGE_RD_2,
  RESOURCE_ID_IMAGE_RD_3,
  RESOURCE_ID_IMAGE_RD_4,
  RESOURCE_ID_IMAGE_RD_5,
  RESOURCE_ID_IMAGE_RD_6,  
};

static const uint32_t BATTERIES[] = {
  RESOURCE_ID_IMAGE_BT_0,
  RESOURCE_ID_IMAGE_BT_10,
  RESOURCE_ID_IMAGE_BT_20,
  RESOURCE_ID_IMAGE_BT_30,
  RESOURCE_ID_IMAGE_BT_40,
  RESOURCE_ID_IMAGE_BT_50,
  RESOURCE_ID_IMAGE_BT_60,
  RESOURCE_ID_IMAGE_BT_70,
  RESOURCE_ID_IMAGE_BT_80,
  RESOURCE_ID_IMAGE_BT_90,
  RESOURCE_ID_IMAGE_BT_100,
};

static uint32_t get_random_bg() {
  int rand_val = rand() % NUM_BGS;
  uint32_t resource = BACKGROUNDS[rand_val];
  return resource;
}

static uint32_t get_random_rd() {
  int rand_val = rand() % NUM_RDS;
  uint32_t resource = ROADS[rand_val];
  return resource;
}

static void battery_callback(BatteryChargeState state) {
  uint8_t battery_level = state.charge_percent / 10;

  // Set bt bitmap and percentage?
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                        "%H:%M" : "%I:%M", tick_time);
  // display it
  text_layer_set_text(s_time_layer, s_buffer);
  
  static char s_date_buffer[16];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%b %d", tick_time);
  
  text_layer_set_text(s_date_layer, s_date_buffer);

  if (DEMO_MODE || tick_time->tm_min % 10 == 0) {
    // update bg
    gbitmap_destroy(s_background_bitmap);
    s_background_bitmap = gbitmap_create_with_resource(get_random_bg());
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    // update rd
    gbitmap_destroy(s_road_bitmap);
    s_road_bitmap = gbitmap_create_with_resource(get_random_rd());
    bitmap_layer_set_bitmap(s_road_layer, s_road_bitmap);
  }
}

static void time_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // time
  s_time_layer = text_layer_create(GRect(0, TIME_Y, bounds.size.w, 60));
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_56));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // date
  s_date_layer = text_layer_create(GRect(0, DATE_Y, bounds.size.w, 32));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DATE_28));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // background
  s_background_layer = bitmap_layer_create(GRect(0, 0, 180, BG_HEIGHT));
  bitmap_layer_set_compositing_mode(s_background_layer, GCompOpSet);
  s_background_bitmap = gbitmap_create_with_resource(get_random_bg());
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);

  // road
  s_road_layer = bitmap_layer_create(GRect(0, BG_HEIGHT, 180, RD_HEIGHT));
  bitmap_layer_set_compositing_mode(s_road_layer, GCompOpSet);
  s_road_bitmap = gbitmap_create_with_resource(get_random_rd());
  bitmap_layer_set_bitmap(s_road_layer, s_road_bitmap);
  
  // car
  s_car_layer = bitmap_layer_create(GRect(CAR_X, CAR_Y, 41, 21));
  bitmap_layer_set_compositing_mode(s_car_layer, GCompOpSet);
  s_car_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CAR);
  bitmap_layer_set_bitmap(s_car_layer, s_car_bitmap);

  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_road_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_car_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  // unload text layers
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_battery_text_layer);

  // unload custom fonts
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
  fonts_unload_custom_font(s_battery_font);
  
  // unload bitmap layers
  bitmap_layer_destroy(s_background_layer);
  bitmap_layer_destroy(s_road_layer);
  bitmap_layer_destroy(s_battery_layer);
  bitmap_layer_destroy(s_car_layer);
  
  // unload gbitmaps
  gbitmap_destroy(s_background_bitmap);
  gbitmap_destroy(s_road_bitmap);
  gbitmap_destroy(s_battery_bitmap);
  gbitmap_destroy(s_car_bitmap);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_set_background_color(s_main_window, PBL_IF_BW_ELSE(GColorWhite, GColorLightGray));

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // set up tick_handler to run every minute
  tick_timer_service_subscribe(DEMO_MODE ? SECOND_UNIT : MINUTE_UNIT, time_handler);
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