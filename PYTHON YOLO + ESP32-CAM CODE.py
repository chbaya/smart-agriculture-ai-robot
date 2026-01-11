import cv2
import serial
from ultralytics import YOLO

# =============================
# SETTINGS
# =============================
ESP32_CAM_URL = "http://192.168.1.105:81/stream"
SERIAL_PORT = "/dev/ttyUSB0"   # Arduino / ESP32
BAUD_RATE = 9600

# =============================
# INIT
# =============================
model = YOLO("yolov8n.pt")
cap = cv2.VideoCapture(ESP32_CAM_URL)
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

print("AI Weed Detection Started")

# =============================
# LOOP
# =============================
while True:
    ret, frame = cap.read()
    if not ret:
        continue

    results = model(frame, conf=0.5)

    weed_detected = False

    for r in results:
        for box in r.boxes:
            cls = int(box.cls[0])
            label = model.names[cls]

            if label.lower() == "weed":
                weed_detected = True
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                cv2.rectangle(frame, (x1,y1), (x2,y2), (0,0,255), 2)
                cv2.putText(frame, "WEED", (x1,y1-10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,0,255), 2)

    if weed_detected:
        ser.write(b'W')   # Spray ON
    else:
        ser.write(b'S')   # Spray OFF

    cv2.imshow("Weed Detection", frame)

    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()
