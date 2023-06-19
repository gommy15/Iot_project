# Reference
https://github.com/petercunha/Emotion

# Emotion
This software recognizes human faces and their corresponding emotions from a video or webcam feed. Powered by OpenCV and Deep Learning.

## Installation

Clone the repository:
```
git clone https://github.com/petercunha/Emotion.git
cd Emotion/
```

Install these dependencies with `pip3 install <module name>`
- tensorflow
- numpy == 1.23.5
- scipy
- opencv-python
- pillow
- pandas
- matplotlib
- h5py
- keras

```
#include <stdio.h>
#include <unistd.h>

// 각 표정에 해당하는 비트 패턴 정의
const unsigned char angry[8]    = {0x3C, 0x42, 0x81, 0xA5, 0x81, 0x81, 0x42, 0x3C};
const unsigned char disgust[8]  = {0x3C, 0x7E, 0x81, 0x99, 0xA5, 0x81, 0x7E, 0x3C};
const unsigned char fear[8]     = {0x66, 0xFF, 0xFF, 0xDB, 0xDB, 0xFF, 0xFF, 0x66};
const unsigned char happy[8]    = {0x18, 0x3C, 0x24, 0x42, 0x42, 0x24, 0x3C, 0x18};
const unsigned char sad[8]      = {0x3C, 0x42, 0xA5, 0x81, 0x81, 0xA5, 0x42, 0x3C};
const unsigned char surprise[8] = {0x3C, 0x7E, 0x99, 0xA5, 0xA5, 0x99, 0x7E, 0x3C};
const unsigned char neutral[8]  = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// 각 표정을 표현하는 구조체 정의
typedef struct {
    const unsigned char* pattern;
    const char* name;
} Expression;

// 표정 배열 정의
Expression expressions[] = {
    {angry, "Angry"},
    {disgust, "Disgust"},
    {fear, "Fear"},
    {happy, "Happy"},
    {sad, "Sad"},
    {surprise, "Surprise"},
    {neutral, "Neutral"}
};

// LED 패턴을 표시하는 함수
void displayPattern(const unsigned char* pattern) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (pattern[row] & (1 << col)) {
                printf("* ");  // LED 활성화
            } else {
                printf("  ");  // LED 비활성화
            }
        }
        printf("\n");
    }
}

int main() {
    int index;

    while (1) {
        printf("표정을 선택하세요 (0-6): ");
        scanf("%d", &index);

        if (index < 0 || index >= sizeof(expressions) / sizeof(expressions[0])) {
            printf("잘못된 선택입니다. 다시 선택하세요.\n");
            continue;
        }

        printf("\n%s:\n", expressions[index].name);
        displayPattern(expressions[index].pattern);
        usleep(1000000);
    }

    return 0;
}
```
