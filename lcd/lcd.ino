#include "Arduino.h"
//The setup function is called once at startup of the sketch
#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
struct can_frame radioPck;

MCP2515 mcp2515(10);

void setup() {
	Serial.begin(115200);
	SPI.begin();

	mcp2515.reset();
	mcp2515.setBitrate(CAN_1000KBPS);
	mcp2515.setNormalMode();

	Serial.println("------- CAN Read ----------");
	Serial.println("ID  DLC   DATA");
	radioOn();
//	enableDisplay();

}

void readMsg() {
	if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
//		if ((canMsg.can_id != 0x3CF) && (canMsg.can_id != 0x3DF)) {

			Serial.print(canMsg.can_id, HEX); // print ID
			Serial.print(" ");
			Serial.print(canMsg.can_dlc, HEX); // print DLC
			Serial.print(" ");

			for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data

				Serial.print(canMsg.data[i], HEX);
				Serial.print(" ");

			}

			Serial.println();
//		}
	}

}

void readKey() {
	if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

//		if ((canMsg.can_id != 0x3CF) && (canMsg.can_id != 0x3DF)) {
//
//			Serial.print(canMsg.can_id, HEX); // print ID
//			Serial.print(" ");
//			Serial.print(canMsg.can_dlc, HEX); // print DLC
//			Serial.print(" ");
//
//			for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data
//
//				Serial.print(canMsg.data[i], HEX);
//				Serial.print(" ");
//
//			}
//			Serial.println();
//		}

		syncLcd();

//		if (not syncLcd()) {
//			Serial.print("not sync ");
		if (canMsg.can_id == 0xA9) {
			Serial.println("keys ");
			Serial.print(canMsg.can_id, HEX); // print ID
			Serial.print(" ");
			Serial.print(canMsg.can_dlc, HEX); // print DLC
			Serial.print(" ");

			for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data

				Serial.print(canMsg.data[i], HEX);
				Serial.print(" ");

			}
			Serial.println();
			if (canMsg.data[2] == 0x00) {
				if (canMsg.data[3] == 0x00) {
					onKeyDown();
				} else if (canMsg.data[3] == 0x01) {
					onKeySrcRight();
				} else if (canMsg.data[3] == 0x02) {
					onKeySrcLeft();
				} else if (canMsg.data[3] == 0x03) {
					onKeyVolUp();
				} else if (canMsg.data[3] == 0x43) {
					onKeyVolUpHold();
				} else if (canMsg.data[3] == 0x04) {
					onKeyVolDown();
				} else if (canMsg.data[3] == 0x44) {
					onKeyVolDownHold();
				} else if (canMsg.data[3] == 0x05) {
					onKeyMute();
				} else if (canMsg.data[3] == 0x85) {
					onKeyMuteHold();
				} else if (canMsg.data[3] == 0x80) {
					onKeyDownHold();
				} else if (canMsg.data[3] == 0x82) {
					onKeySrcLefttHold();
				} else if (canMsg.data[3] == 0x81) {
					onKeySrcRightHold();
				} else if (canMsg.data[3] == 0x40) {
					onKeyHold2();
				}

			} else if (canMsg.data[2] == 0x01) {
				if (canMsg.data[3] == 0x01) {
					onRolDown();
				} else if (canMsg.data[3] == 0x41) {
					onRolUp();
				}
			}

//			}

		}

	}

}

void readMsg2() {
	if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

		Serial.print(canMsg.can_id, HEX); // print ID
		Serial.print(" ");
		Serial.print(canMsg.can_dlc, HEX); // print DLC
		Serial.print(" ");

		for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data

			Serial.print(canMsg.data[i], HEX);
			Serial.print(" ");

		}

		Serial.println();

	}

}

bool readMs() {
	if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

		if ((canMsg.can_id == 0x521)) {

			Serial.print(canMsg.can_id, HEX); // print ID
			Serial.print(" ");
			Serial.print(canMsg.can_dlc, HEX); // print DLC
			Serial.print(" ");

			for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data

				Serial.print(canMsg.data[i], HEX);
				Serial.print(" ");

			}

			Serial.println();
			return true;
		}
	}
	return false;

}
void enableDisplay() {
	can_frame enableDispMsg = { 0x04, 0x52, 0x02, 0xFF, 0xFF, 0x81, 0x81, 0x81 };
	enableDispMsg.can_dlc = 8;
	enableDispMsg.data[0] = 0x04;
	enableDispMsg.data[1] = 0x52;
	enableDispMsg.data[2] = 0x02;
	enableDispMsg.data[3] = 0xFF;
	enableDispMsg.data[4] = 0xFF;
	enableDispMsg.data[5] = 0x81;
	enableDispMsg.data[6] = 0x81;
	enableDispMsg.data[7] = 0x81;

	mcp2515.sendMessage(&enableDispMsg);
}

void syncLcd() {
//3DF 8 79 0 81 81 81 81 81 81

//		if (canMsg.can_id != 0x3CF) {
	radioPck.can_id = canMsg.can_id + 0x400;
	radioPck.can_dlc = 8;
	radioPck.data[0] = 0x79;
	radioPck.data[1] = 0x00;
	radioPck.data[2] = 0x81;
	radioPck.data[3] = 0x81;
	radioPck.data[4] = 0x81;
	radioPck.data[5] = 0x81;
	radioPck.data[6] = 0x81;
	radioPck.data[7] = 0x81;
	mcp2515.sendMessage(&radioPck);
//		}

//	if (canMsg.can_id != 0x3CF) {
//		radioPck.can_id = 0x3DF;
//		radioPck.can_dlc = 8;
//		radioPck.data[0] = 0x79;
//		radioPck.data[1] = 0x00;
//		radioPck.data[2] = 0x81;
//		radioPck.data[3] = 0x81;
//		radioPck.data[4] = 0x81;
//		radioPck.data[5] = 0x81;
//		radioPck.data[6] = 0x81;
//		radioPck.data[7] = 0x81;
//		mcp2515.sendMessage(&radioPck);
//	} else

//	if (canMsg.can_id != 0x1C1) {
//		radioPck.can_id = 0x5C1;
//		radioPck.can_dlc = 8;
//		radioPck.data[0] = 0x74;
//		radioPck.data[1] = 0x91;
//		radioPck.data[2] = 0x81;
//		radioPck.data[3] = 0x81;
//		radioPck.data[4] = 0x81;
//		radioPck.data[5] = 0x81;
//		radioPck.data[6] = 0x81;
//		radioPck.data[7] = 0x81;
//		mcp2515.sendMessage(&radioPck);
//		return true;
//	} else 	if (canMsg.can_id != 0x0A9) {
//		radioPck.can_id = 0x4A9;
//		radioPck.can_dlc = 8;
//		radioPck.data[0] = 0x79;
//		radioPck.data[1] = 0x81;
//		radioPck.data[2] = 0x81;
//		radioPck.data[3] = 0x81;
//		radioPck.data[4] = 0x81;
//		radioPck.data[5] = 0x81;
//		radioPck.data[6] = 0x81;
//		radioPck.data[7] = 0x81;
//		mcp2515.sendMessage(&radioPck);
//		return true;
//	}
//	return false;
}

void radioOn() {
//3DF 8 79 0 81 81 81 81 81 81
	Serial.println("Radio on");
	radioPck.can_id = 0x1B1;
	radioPck.can_dlc = 8;
	radioPck.data[0] = 0x08;
	radioPck.data[1] = 0x04;
	radioPck.data[2] = 0x52;
	radioPck.data[3] = 0xFF;
	radioPck.data[4] = 0xFF;
	radioPck.data[5] = 0x81;
	radioPck.data[6] = 0x81;
	radioPck.data[7] = 0x81;
	if (mcp2515.sendMessage(&radioPck) != MCP2515::ERROR_OK) {
		Serial.println("Radio on Error");
	};

}

void show_text(char old[8], char newx[12]) {
	struct can_frame packets[4];
	uint8_t i;

	/* Budujemy pakiety */
	packets[0].can_id = 0x121;
//	packets[0].rtr = 0;
	packets[0].can_dlc = 8;
	packets[0].data[0] = 0x10;
	packets[0].data[1] = 0x19;
	packets[0].data[2] = 0x76;
	packets[0].data[3] = 0x60;
	packets[0].data[4] = 0x01;
	packets[0].data[5] = old[0];
	packets[0].data[6] = old[1];
	packets[0].data[7] = old[2];

	packets[1].can_id = 0x121;
//	packets[1].rtr = 0;
	packets[1].can_dlc = 8;
	packets[1].data[0] = 0x21;
	packets[1].data[1] = old[3];
	packets[1].data[2] = old[4];
	packets[1].data[3] = old[5];
	packets[1].data[4] = old[6];
	packets[1].data[5] = old[7];
	packets[1].data[6] = 0x10;
	packets[1].data[7] = newx[0];

	packets[2].can_id = 0x121;
//	packets[2].rtr = 0;
	packets[2].can_dlc = 8;
	packets[2].data[0] = 0x22;
	packets[2].data[1] = newx[1];
	packets[2].data[2] = newx[2];
	packets[2].data[3] = newx[3];
	packets[2].data[4] = newx[4];
	packets[2].data[5] = newx[5];
	packets[2].data[6] = newx[6];
	packets[2].data[7] = newx[7];

	packets[3].can_id = 0x121;
//	packets[3].rtr = 0;
	packets[3].can_dlc = 8;
	packets[3].data[0] = 0x23;
	packets[3].data[1] = newx[8];
	packets[3].data[2] = newx[9];
	packets[3].data[3] = newx[10];
	packets[3].data[4] = newx[11];
	packets[3].data[5] = 0x00;
	packets[3].data[6] = 0x81;
	packets[3].data[7] = 0x81;

	for (i = 0; i < 4; i++) {
		mcp2515.sendMessage(&packets[i]);
		delay(1);
	}

}

void showMessage(String text) {
	Serial.println("Message: " + text);
	char lowDisp[8];
	char highDisp[12];
	String t = text;
	while (t.length() < 12) {
		t = t + " ";
	}

	lowDisp[0] = text.charAt(0);
	lowDisp[1] = text.charAt(1);
	lowDisp[2] = text.charAt(2);
	lowDisp[3] = text.charAt(3);
	lowDisp[4] = text.charAt(4);
	lowDisp[5] = text.charAt(5);
	lowDisp[6] = text.charAt(6);
	lowDisp[7] = text.charAt(7);

	highDisp[0] = text.charAt(0);
	highDisp[1] = text.charAt(1);
	highDisp[2] = text.charAt(2);
	highDisp[3] = text.charAt(3);
	highDisp[4] = text.charAt(4);
	highDisp[5] = text.charAt(5);
	highDisp[6] = text.charAt(6);
	highDisp[7] = text.charAt(7);
	highDisp[8] = text.charAt(8);
	highDisp[9] = text.charAt(9);
	highDisp[10] = text.charAt(10);
	highDisp[11] = text.charAt(11);

	show_text(lowDisp, highDisp);

}

void shortMessage(String text) {
	showMessage(text);
//	syncLcd();
//	delay(1000);
//	showMessage(" ");
//	syncLcd();
}

void loop() {
//	Serial.println(0x121 + 0x400,HEX);
//	Serial.print("");

	readMsg();
//	readKey();
//	mcp2515.sendMessage(&radioPck);
//	showMessage("DANIEL");
//	delay(1000);
//

//	showMessage("CHUJU-");
//	delay(1000);

//	showMessage("YEAR");
//	delay(1000);
//
//	showMessage("ZYCZY");
//	delay(1000);
//	showMessage("ALBERT");
//	delay(1000);

}

void onKeyVolUp() {
	Serial.println("onKeyVolUp");
	shortMessage("VOL UP");
}

void onKeyVolUpHold() {
	Serial.println("onKeyVolUpHold");
	shortMessage("VOL UP");
}

void onKeyVolDown() {
	Serial.println("onKeyVolDown");
	shortMessage("VOL DOWN");
}

void onKeyVolDownHold() {
	Serial.println("onKeyVolDownHold");
	shortMessage("VOL DOWN");
}

void onKeyMute() {
	Serial.println("onKeyMute");
	shortMessage("PAUSE");
}

void onKeyMuteHold() {
	Serial.println("onKeyMuteHold");
	shortMessage("PAUSE");
}

void onKeySrcLeft() {
	Serial.println("onKeySrcLeft");
	shortMessage("SRC LEFT");
}

void onKeySrcRight() {
	Serial.println("onKeySrcRight");
	shortMessage("SRC RIGHT");
}

void onKeyDown() {
	Serial.println("onKeyDown");
	shortMessage("KEY LOAD");
}

void onRolUp() {
	Serial.println("onRolUp");
	shortMessage("ROL UP");
}

void onRolDown() {
	Serial.println("onRolDown");
	shortMessage("ROL DOWN");
}

void onKeyDownHold() {
	Serial.println("onKeyDownHold");
	shortMessage("HOLD1");
}

void onKeySrcLefttHold() {
	Serial.println("onKeySrcLefttHold");
	shortMessage("HOLD1");
}

void onKeySrcRightHold() {
	Serial.println("onKeySrcRightHold");
	shortMessage("HOLD1");
}

void onKeyHold2() {
	Serial.println("onKeyHold2");
	shortMessage("HOLD2");
}
