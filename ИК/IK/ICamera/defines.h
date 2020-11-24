#ifndef DEFINES
#define DEFINES

// Image parameters
#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define IMG_SIZE IMG_WIDTH*IMG_HEIGHT

#define TEMP_INPUT_SIZE 480
#define ADC_SIZE 65536

// Type of Messages
#define LOG 0
#define MSG_BOX_INFORMATION 1
#define MSG_BOX_WARNING 2
#define MSG_BOX_CRITICAL 3

//triplebuf.h
#define BUFFERS_PAGES 3

//pcaprecieve
#define MODE_FILTER_HOST 0
#define MODE_FILTER_ARP_UDP 1

//dialog_menu2_video_settings

#define LINES_NUMBER 22
#define RIGHT_CENTER_TO_LEFT_CENTER 0
//   **********  **************
//   *********0  0*************
//   **********  **************

#define LEFT_CENTER_TO_LEFT_CENTER 1
//   **********
//   1*********
//   **********

//   **********
//   1*********
//   **********

#define RIGHT_CENTER_TO_RIGHT_CENTER 2
//   **********
//   *********2
//   **********

//   **********
//   *********2
//   **********
#define RIGHT_CENTER_TO_POINT_UP    3
#define RIGHT_CENTER_TO_POINT_DOWN  4
#define RIGHT_CENTER_TO_POINT_RIGHT 5
#define LEFT_CENTER_TO_POINT_UP     6
#define LEFT_CENTER_TO_POINT_DOWN   7
#define LEFT_CENTER_TO_POINT_LEFT   8



// <- Left shift
#define LEFT_SHIFT 4

#define LINE_5_LENTH  30
#define LINE_11_LENTH 30

#define TYPE_ONE_RAW_TEST_VIDEO      0x01
#define TYPE_ONE_CALIBRATED_VIDEO    0x02

#define TYPE_TWO_NONE_CALIBRATE      0x10
#define TYPE_TWO_ONE_POINT_CALIBRATE 0x20
#define TYPE_TWO_TWO_POINT_CALIBRATE 0x40

#define TYPE_THREE_FILTERS_SNR       0x100
#define TYPE_THREE_FILTERS_NARCISS   0x200


//UDP COMMANDS ADR

// CMD structure
// data[0] = 0x00; preambula
// data[1] = 0x00; preambula
// data[2] = adr_MSB
// data[3] = adr_LSB
// data[4] = cmd_MSB
// ...
// data[7] = cmd_LSB
#define PCKT_TYPE_UDP     0x00
#define PCKT_TYPE_ARP     0x01

#define CMD_LEN 8

#define ADR_VIDEO_STREAM_TYPE   0x0100
#define AVST_TEST_ON           0x02
#define AVST_CALIBRATED_ON     0x05
#define AVST_OFF               0x00

//Calibration commands

// turn on off sensor
#define ADR_SENSOR_POWER       0x0102
#define ASP_SENSOR_ON          0x4000
#define ASP_SENSOR_OFF         0x8000

//Filtration and Algorithms
#define ADR_SNR_DDE                  0x070F
#define ASD_SNR_ON                   0x01
#define ASD_DDE_ON                   0x10
#define ASD_AFTER_GUIDED             0x100
#define ASD_PIP                      0x1000

#define ADR_SNR_ROW_EPS                 0x0704
// data [31:0] = round(0.000001*2^32)*49
#define VAL_SNR_ROW_EPS(x) (round(x*(2^32)))*49

#define ADR_SNR_COLUMN_EPS              0x0705
// data [31:0] = round(0.1*2^32)
#define ADR_DDE_NORMAL_EPS              0x0706
// data [31:0] = round(0.0001*2^32)*81
#define ADR_DDE_WEAK_EPS                0x0707
// data [31:0] = round(0.0000001*2^32)*81
#define ADR_DDE_GAIN_MAX                0x0709
// data [8:0] default 50


#endif // DEFINES

