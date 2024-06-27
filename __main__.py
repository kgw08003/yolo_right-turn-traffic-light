import torch
import cv2
import numpy as np
import serial

# 시리얼 통신 설정
seri = serial.Serial(port='COM4',
                    baudrate=9600,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    bytesize=serial.EIGHTBITS)

# YOLOv5 모델 로드
model = torch.hub.load('ultralytics/yolov5', 'yolov5s')

# 객체 탐지 함수
def detect_object(image):
    # 객체 탐지
    results = model(image)

    # 탐지된 객체 추출
    detections = results.xyxy[0].detach().numpy()
    # 일부 GPU 사용, CPU 호출한 후 numpy 배열로 변환

    # 객체 탐지 결과 초기화
    object_positions = []
    object_classes = []

    # 탐지된 객체 반복
    for detection in detections:
        class_id = int(detection[5])
        confidence = detection[4]

        # 신뢰도가 일정 이상인 객체만 선택
        if confidence > 0.2:
            center_x = int((detection[0] + detection[2]) / 2)
            center_y = int((detection[1] + detection[3]) / 2)
            object_positions.append((center_x, center_y))
            object_classes.append(class_id)

    return object_positions, object_classes

# 메인 함수
def main():
    cap = cv2.VideoCapture(0)  # 웹캠 사용을 위한 비디오 캡처 객체 생성
    if not cap.isOpened():
        print("Error: Failed to open webcam.")
        return

    while True:
        ret, frame = cap.read()  # 프레임 읽기
        if not ret:
            print("Error: Failed to capture frame.")
            break

        # 객체 탐지
        object_positions, object_classes = detect_object(frame)
        
        # 사람이 탐지된 경우 LED0 켜기
        if 0 in object_classes:
            sendToArduino(0)
        else:
            # 사람이 탐지되지 않은 경우 LED1 켜기
            sendToArduino(1)

        # 화면에 프레임 표시
        cv2.imshow('Object Detection', frame)

        # 'q' 키를 누르면 종료
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # 비디오 캡처 객체 해제
    cap.release()
    cv2.destroyAllWindows()

# 아두이노로 LED 상태 전송하는 함수
def sendToArduino(led_status):
    comm = 'LED' + str(led_status)
    print('Send:', comm)
    comm += '\n'
    seri.write(bytes(comm, encoding='ascii'))

if __name__ == "__main__":
    main()
