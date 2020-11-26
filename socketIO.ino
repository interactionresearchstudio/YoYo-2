void socketIO_offline(const char * payload, size_t length) {
  Serial.println("partner is offline");
  delay(50);
  isIncreasing = prevIncreasing;
  setPosition(prevPos);
}

void socketIO_Connected(const char * payload, size_t length) {
  Serial.println("Socket.IO Connected!");
  blinkOnConnect();
}

void socketIO_sendMac(const char * payload, size_t length) {
  Serial.println("GOT MAC REQUEST");
  const size_t capacity = JSON_OBJECT_SIZE(1) + 50;
  DynamicJsonDocument doc(capacity);
  doc["macAddress"] =  myID;
  String bodyReq;
  serializeJson(doc, bodyReq);
  Serial.println(bodyReq);
  socketIO.emit("mac", bodyReq.c_str());
}

void socketIO_event(const char * payload, size_t length) {
  Serial.print("got message: ");
  Serial.println(payload);
}

void socketIO_msg(const char * payload, size_t length) {
  Serial.println("got msg");
  const size_t capacity = 2 * JSON_OBJECT_SIZE(2) + 60;
  DynamicJsonDocument incomingDoc(capacity);
  deserializeJson(incomingDoc, payload);
  const char* recMacAddress = incomingDoc["macAddress"];
  const char* data_project = incomingDoc["data"]["project"];

  Serial.print("I got a message from ");
  Serial.println(recMacAddress);
  Serial.print("Which is of type ");
  Serial.println(data_project);

  if (String(data_project) == "test") {
    blinkDevice();
  } else if (String(data_project) == "YoYo2") {
    prevPos = getPosition();
    prevIncreasing = isIncreasing;

    long servo_pos = incomingDoc["data"]["position"];
    isIncreasing = bitRead(servo_pos, 7);
    bitWrite(servo_pos, 7, 0);
    setPosition(servo_pos);
  }
}

void socketIO_sendButtonPress() {
  Serial.println("button send");
  const size_t capacity = 2 * JSON_OBJECT_SIZE(2);
  DynamicJsonDocument doc(capacity);
  doc["macAddress"] = getRemoteMacAddress(1);
  JsonObject data = doc.createNestedObject("data");
  data["project"] = "test";
  String sender;
  serializeJson(doc, sender);
  socketIO.emit("msg", sender.c_str());
}

void socketIO_sendServo() {
  Serial.println("Servo send");
  const size_t capacity = 3 * JSON_OBJECT_SIZE(2);
  DynamicJsonDocument doc(capacity);
  doc["macAddress"] = getRemoteMacAddress(1);
  JsonObject data = doc.createNestedObject("data");
  data["project"] = "YoYo2";
  byte currentPos = getPosition();
  if (isIncreasing) bitWrite(currentPos, 7, 1);
  else bitWrite(currentPos, 7, 0);
  data["position"] = String(currentPos);
  String sender;
  serializeJson(doc, sender);
  socketIO.emit("msg", sender.c_str());
}

void setupSocketIOEvents() {
  // Setup 'on' listen events
  socketIO.on("connect", socketIO_Connected);
  socketIO.on("event", socketIO_event);
  socketIO.on("send mac", socketIO_sendMac);
  socketIO.on("msg", socketIO_msg);
  socketIO.on("partner offline", socketIO_offline);
  socketIO.begin(host, port, path);
  Serial.println("attached socketio listeners");
}
