Yolov5를 활용해서 AI 기반의 우회전 차량으로 인한 교통사고 예방시스템을 개발한것으로

AI를 기반으로 사람을 탐지하여 우회전 신호등의 불을 제어하는 인공지능 기반 우회전 신호등을 제작한 것이다.

yolo_right_turn.zip 파일 + yolov5 공식 깃허브 자료를 다운받아 해당하는 MCU에서 도트매트릭스 코드를 우선 실행 후 해당 코드를 실행시켜주면 정상적으로 작동함.

도트매트릭스는 32*16 도트매트릭스로 LK 임베디드사 것을 사용하여 제작하였음.
도트매트릭스 코드에서는 빨간원과 초록 화살표를 각각 출력하도록하여 카메라로 사람객체를 인식했을 경우 해당 차량이 정지할 수 있도록 빨간 원을 출력하고, 
사람객체를 인식하지 못하는 경우는 진행등인 초록 화살표를 출력하도록 코드를 구성하였다.

Yolo를 이용하여 카메라를 통해 사람 객체를 인식시켜 사람 객체를 인식한 경우에만 해당 신호들의 신호를 제어하도록하여 

MCU는 PC환경, 라즈베리파이 3, 라즈베리파이 4, jetson nano로 구성되어 있으며 세부적으로 살펴보면

__main__.py 파일은 욜로 동작 결과를 화면에 출력하여 사람 감지시 도트매트릭스의 신호를 제어하도록 만든 가장 기본 모델
__main_gray.py 파일은 그레이 스케일로 화면을 처리하여 처리 속도를 높인것
__main__NC.py 파일은 No Camera로 화면이 출력되지 않고 동작 처리만 하도록 구성한것

pc환경에서는 노트북 웹캠을 이용하였고, 나머지 MCU에서는 화상웹캠을 활용하여 구현하였다.

![image](https://github.com/kgw08003/yolo_right-turn-traffic-light/assets/109195054/0634888b-7123-4721-b5b0-cb47abbf93c9)
이런식으로 화면에 사람이 탐지될 경우 해당 우회전 신호등의 정지 신호가 출력되도록 만든 것이다.

![3D가상공간](https://github.com/kgw08003/yolo_right-turn-traffic-light/assets/109195054/6580f18b-bbe8-4ca8-bb7c-8a4be465eb7d)
해당 사진은 3D가상공간을 만들어본 사진으로 횡단보도내 사람이 카메라를 통해 탐지되면 신호를 제어하는 가상 공간을 만들어보았다.

mp4 영상을 열어보면 우리가 제작하고자한 방향성과 결과물을 시뮬레이션으로 만든 영상이 나온다.
