#include <pebble.h>

int index = 0;
int KEY_INDEX = 1;

static void sendOne() {
	if (index++ < 30) {
		DictionaryIterator *iter;
		AppMessageResult result = app_message_outbox_begin(&iter);
		if (result == APP_MSG_OK) {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sending %d", index);
			Tuplet value = TupletInteger(KEY_INDEX, index);
			dict_write_tuplet(iter, &value);
			dict_write_end(iter);
			app_message_outbox_send();
		} else {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send %d", index);
		}
	}
}

static void out_sent_handler(DictionaryIterator *sent, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "We got an ack!");
	sendOne();
}


static void in_received_handler(DictionaryIterator *iter, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "javascript is ready");
	sendOne();
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
	int max = app_message_inbox_size_maximum();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped! %d, max=%d", reason, max);
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
}

static Window *menuWindow;


static void handle_init(void) {
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_failed(out_failed_handler);
	app_message_register_outbox_sent(out_sent_handler);
	const uint32_t inbound_size = app_message_inbox_size_maximum();
	const uint32_t outbound_size = app_message_outbox_size_maximum();

	app_message_open(inbound_size, outbound_size);

	menuWindow = window_create();
	window_set_window_handlers(menuWindow, (WindowHandlers) {
		.load = NULL,
		.unload = NULL,
	});
	window_stack_push(menuWindow, true);

}

static void handle_deinit(void) {
	window_destroy(menuWindow);
}
	
	int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
