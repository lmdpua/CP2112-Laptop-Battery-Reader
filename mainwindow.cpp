#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "types.h"
#include "smbus.h"
#include "SLABCP2112.h"
#include "cp2112.h"
#include "BatteryCommands.h"
#include "SMBusConfig.h"
#include <QThread>
#include <bitset>
//#include <QPalette>
//#include <QString>
//#include <string.h>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CP2112 Laptop Battery Reader");
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(1000); // И запустим таймер
}

MainWindow::~MainWindow()
{
    delete ui;
}

//#define VID 0x10C4
//#define PID 0xEA90

HID_SMBUS_DEVICE    m_hidSmbus;
INT                 DevNumbers;
INT                 DevNumber;

void MainWindow::on_pushButton_clicked()
{
    DevNumber=0;
}

void MainWindow::slotTimerAlarm()
{
    DevNumbers = CP2112_Find();
    if (DevNumbers==0)
    {
        ui->statusbar->showMessage("CP2112 not connected");
        return;
    }
//    else
//    {
//        QString str = QString::number(DevNumbers);
//        ui->statusbar->showMessage(str+ "pcs of CP2112 connected);
//    }


    if (!CP2112_Open(0,&m_hidSmbus))
    {
        ui->statusbar->showMessage("CP2112 not connected");
        HidSmbus_Close(m_hidSmbus);
//        HidSmbus_Reset(m_hidSmbus);
        return;
    }
    else
    {
        CP2112_SetConfig(m_hidSmbus); //Передаем конфиг на устройство

        //Теперь читаем конфиг с устройства, чтобы убедится, что он записался
        DWORD bitRate;              //Для этого нужно объявить кучу переменных
        BYTE address;
        BOOL autoReadRespond;
        WORD writeTimeout;
        WORD readTimeout;
        BOOL sclLowTimeout;
        WORD transferRetries;
        //И вот только теперь читаем конфиг
        HidSmbus_GetSmbusConfig(m_hidSmbus, &bitRate,
                                &address, &autoReadRespond,
                                &writeTimeout, &readTimeout,
                                &sclLowTimeout, &transferRetries);

//       if (bitRate !=


        HID_SMBUS_DEVICE_STR serial={0};
        HID_SMBUS_DEVICE_STR manufacturer={0};
        HID_SMBUS_DEVICE_STR product={0};
        HidSmbus_GetOpenedString(m_hidSmbus,serial,HID_SMBUS_GET_SERIAL_STR);
        HidSmbus_GetOpenedString(m_hidSmbus,manufacturer,HID_SMBUS_GET_MANUFACTURER_STR);
        HidSmbus_GetOpenedString(m_hidSmbus,product,HID_SMBUS_GET_PRODUCT_STR);


        QString str="CP2112 connected | s/n: ";
        str+=QString::fromStdString(serial);
        str+=" | Manufacturer: ";
        str+=QString::fromStdString(manufacturer);
        str+=" | Product: ";
        str+=QString::fromStdString(product);
        str+=" | SMBus Speed: ";
        str+=QString::number(bitRate);
        str+="Hz";

        ui->statusbar->showMessage(str);





        QLabel *SmartBatteryInterface [37] = {
                                                ui->data_0x00,ui->data_0x01,ui->data_0x02,
                                                ui->data_0x03,ui->data_0x04,ui->data_0x05,
                                                ui->data_0x06,ui->data_0x07,ui->data_0x08,
                                                ui->data_0x09,ui->data_0x0a,ui->data_0x0b,
                                                ui->data_0x0c,ui->data_0x0d,ui->data_0x0e,
                                                ui->data_0x0f,ui->data_0x10,ui->data_0x11,
                                                ui->data_0x12,ui->data_0x13,ui->data_0x14,
                                                ui->data_0x15,ui->data_0x16,ui->data_0x17,
                                                ui->data_0x18,ui->data_0x19,ui->data_0x1a,
                                                ui->data_0x1b,ui->data_0x1c,ui->data_0x20,
                                                ui->data_0x21,ui->data_0x22,ui->data_0x23,
                                                ui->data_0x3c,ui->data_0x3d,ui->data_0x3e,
                                                ui->data_0x3f
                                             };

        const INT Addresses [NUMBER_OF_SBS_ADDRESSES] =
        {
            0x00,   // MANUFACTURER_ACCESS
            0x01,   // REMAINING_CAPACITY_ALARM
            0x02,   // REMAINING_TIME_ALARM
            0x03,   // BATTERY_MODE
            0x04,   // AT_RATE
            0x05,   // AT_RATE_TIME_TO_FULL
            0x06,   // AT_RATE_TIME_TO_EMPTY
            0x07,   // AT_RATE_OK
            0x08,   // TEMPERATURE
            0x09,   // VOLTAGE
            0x0a,   // CURRENT
            0x0b,   // AVERAGE_CURRENT
            0x0c,   // MAX_ERROR
            0x0d,   // RELATIVE_STATE_OF_CHARGE
            0x0e,   // ABSOLUTE_STATE_OF_CHARGE
            0x0f,   // REMAINING_CAPACITY
            0x10,   // FULL_CHARGE_CAPACITY
            0x11,   // RUN_TIME_TO_EMPTY
            0x12,   // AVERAGE_TIME_TO_EMPTY
            0x13,   // AVERAGE_TIME_TO_FULL
            0x14,   // CHARGING_CURRENT
            0x15,   // CHARGING_VOLTAGE
            0x16,   // BATTERY_STATUS
            0x17,   // CYCLE_COUNT
            0x18,   // DESIGN_CAPACITY
            0x19,   // DESIGN_VOLTAGE
            0x1a,   // SPECIFICATION_INFO
            0x1b,   // MANUFACTURER_DATE
            0x1c,   // SERIAL_NUMBER
            0x20,   // MANUFACTURER_NAME
            0x21,   // DEVICE_NAME
            0x22,   // DEVICE_CHEMISTRY
            0x23,   // MANUFACTURER_DATA
            0x3c,   // CELL4_VOLTAGE
            0x3d,   // CELL3_VOLTAGE
            0x3e,   // CELL2_VOLTAGE
            0x3f    // CELL1_VOLTAGE
        };

        for (int i = 0; i<NUMBER_OF_SBS_ADDRESSES; i++)
        {
            QString str="";
            INT     data_word;
            INT     addr=Addresses[i];
            BOOL    lenovo = 0;

            switch (addr)
            {

            case 0x08:  // TEMPERATURE
            {
                    if (ReadWord(m_hidSmbus, &data_word, Addresses[i]))
                    {
                        str = QString::number((float)data_word/10.0 - 273.15); //Преобразование из Кельвин в Цельсий
                        SmartBatteryInterface[i]->setText(str);
                    }
                    else SmartBatteryInterface[i]->clear();
            }
                break;

            case 0x1b:  // MANUFACTURER_DATE
            {
                    if (ReadWord(m_hidSmbus, &data_word, Addresses[i]))
                    {
                        QDate date; //Объявляем переменную date
                        UINT16 day = 0b00011111 & data_word;
                        UINT16 month = data_word >> 5 & 0b00001111;
                        UINT16 year = 1980 + (data_word >> 9 & 0b01111111);
                        date.setDate(year, month, day); //Инициализируем переменную date полученными значениями
                        str = date.toString("dd-MMM-yyyy"); //Преобразование с форматированием даты
                        SmartBatteryInterface[i]->setText(str);
                    }
                    else SmartBatteryInterface[i]->clear();
            }
                break;

            case 0x20:  // MANUFACTURER_NAME
            case 0x21:  // DEVICE_NAME
            case 0x22:  // DEVICE_CHEMISTRY
            {
                    INT     length_of_block=0;
                    char    readed_block[32]={0};

                    if (ReadBlock(m_hidSmbus, readed_block, &length_of_block, Addresses[i]))
                    {
                        std::string block_string (readed_block, length_of_block);
                        str = QString::fromStdString(block_string);
                        SmartBatteryInterface[i]->setText(str);
                    }
                    else SmartBatteryInterface[i]->clear();
            }
                break;

            case 0x23:  // MANUFACTURER_DATA
            {
                    INT     length_of_block=0;
                    BYTE    readed_block[32]={0};


                    if (ReadDataBlock(m_hidSmbus, readed_block, &length_of_block, Addresses[i]))
                    {
                        for (BYTE i=0;i<length_of_block;i++)    //Перебираем все значения массива
                        {
                            QString number = QString("%1") //Строка, в которой будет один аргумент
                                             .arg(readed_block[i], 2, 16, QLatin1Char('0')) //Преобразуем значение в HEX с лидирующим нулем
                                             .toUpper();    //Все символы в верхнем регистре
                            str+=number;                    //Добавляем полученное значение в итоговую строку
                            str+=" ";                       //Добавляем пробел
                        }
                        SmartBatteryInterface[i]->setText(str);
                    }
                    else SmartBatteryInterface[i]->clear();
            }
                break;

            case 0x3c:   // CELL4_VOLTAGE//check
            {
                INT     length_of_block=0;
                BYTE    readed_block[32]={0};
                //Создадим небольшой массив указателей для управления надписями
                QLabel *cell_voltage_label [4] = {
                    ui->label_0x3c,
                    ui->label_0x3d,
                    ui->label_0x3e,
                    ui->label_0x3f
                };

                // Прочтем значение по адресу CELL4_VOLTAGE как массив
                if (ReadDataBlock(m_hidSmbus, readed_block, &length_of_block, Addresses[i]))
                {
                    for(BYTE i=0; i<length_of_block; i++) // Сравним полученную строку с шаблонами
                    {
                        if(memcmp(&readed_block[i],"LENOVO" ,6)==0)
                        {
                            lenovo=TRUE;
                            break;
                        }
                        else if(memcmp(&readed_block[i],"Lenovo" ,6)==0)
                        {
                            lenovo=TRUE;
                            break;
                        }
                        else if(memcmp(&readed_block[i],"IBM" ,3)==0)
                        {
                            lenovo=TRUE;
                            break;
                        }
                        else
                            lenovo=FALSE;
                    }

                    if (lenovo)//Если батарея Lenovo, то значение напряжения ячеек находится в MANUFACTURER_DATA
                    {
                        if(!ReadDataBlock(m_hidSmbus, readed_block, &length_of_block, 0x23))break; //По адресу 0x23 находится MANUFACTURER_DATA
                        for (BYTE n=0; n<4; n++)
                        {
                            data_word = (readed_block[11-n*2]<<8)|readed_block[10-n*2]; //  Данные напряжения
                            //  четвертой ячейки находятся в 11м и 10м байте массива,
                            //  третьей ячейки - в 9м и 8м,
                            //  второй ячейки - в 7м и 6м,
                            //  первой ячейки - в 5м и 4м
                            str = QString::number(data_word);
                            SmartBatteryInterface[i+n]->setText(str);           // Отображаем напряжения

                            str=QString("Cell%1Voltage %2 and %3 from (0x23)")
                                    .arg(4-n)
                                    .arg((11-2*n),2,10,QLatin1Char('0'))    //  Десятичное число из двух знаков
                                    .arg((10-2*n),2,10,QLatin1Char('0'));   //  с лидирующим нулем

                            cell_voltage_label[n]->setText(str);                //  Изменяем описание
                        }
                    }
                    else //Если батарея не Lenovo, то стандартный метод
                    {
                        for (BYTE n=0; n<4; n++)
                        {
                            if (ReadWord(m_hidSmbus, &data_word, Addresses[i+n]))
                            {
                                str = QString::number(data_word);
                                SmartBatteryInterface[i+n]->setText(str);

                                str=QString("Cell%1Voltage() (0x23)").arg(4-n); //  Вернем стандартную надпись
                                cell_voltage_label[n]->setText(str);
                            }
                            else SmartBatteryInterface[i+n]->clear();
                        }

                    }

                }
                else
                {
                    for (BYTE n=0; n<4; n++)
                    {
                        SmartBatteryInterface[i+n]->clear();
                    }

                }
                i+=3;   // Перепрыгнем в цикле for (int i = 0; i<NUMBER_OF_SBS_ADDRESSES; i++)
                // Так как уже прочитали напряжение всех четырех элементов

            }
                break;

            default:
                {
                    if (ReadWord(m_hidSmbus, &data_word, Addresses[i]))
                    {
                        str = QString::number(data_word);
                        SmartBatteryInterface[i]->setText(str);
                    }
                    else SmartBatteryInterface[i]->clear();
                 }
                break;
            }
        }

        //Отображаем значения битов в байте BATTERY_MODE
        for (BYTE i=0; i<16;i++)
        {
#define     BATTERY_MODE_ADDRESS 0x03
            INT     bitFlags;
            QLabel *BatteryModeBitFlag [16] = { ui->bm_00,ui->bm_01,ui->bm_02, ui->bm_03,
                                                ui->bm_04,ui->bm_05,ui->bm_06, ui->bm_07,
                                                ui->bm_08,ui->bm_09,ui->bm_10, ui->bm_11,
                                                ui->bm_12,ui->bm_13,ui->bm_14, ui->bm_15,
                                              };
            if (ReadWord(m_hidSmbus, &bitFlags, BATTERY_MODE_ADDRESS))
            {
                std::bitset<16> bits = bitFlags;
                BOOL y;
                y=bits.test(i);
                switch (y)
                {
                case 1:
                BatteryModeBitFlag[i]->setStyleSheet("background-color: red");
                    break;
                case 0:
                   BatteryModeBitFlag[i]->setStyleSheet("background-color: green");
                   break;
                }
            }
            else BatteryModeBitFlag[i]->setStyleSheet("default");
        }


        //Отображаем значения битов в байте BATTERY_STATUS
        for (BYTE i=0; i<16;i++)
        {
#define     BATTERY_STATUS_ADDRESS 0x16
            INT     bitFlags;
            QLabel *BatteryStatusBitFlag [16] = { ui->bs_00,ui->bs_01,ui->bs_02, ui->bs_03,
                                                  ui->bs_04,ui->bs_05,ui->bs_06, ui->bs_07,
                                                  ui->bs_08,ui->bs_09,ui->bs_10, ui->bs_11,
                                                  ui->bs_12,ui->bs_13,ui->bs_14, ui->bs_15,
                                                };
            if (ReadWord(m_hidSmbus, &bitFlags, BATTERY_STATUS_ADDRESS))
            {
                std::bitset<16> bits = bitFlags;
                BOOL y;
                y=bits.test(i);
                switch (y)
                {
                case 1:
                BatteryStatusBitFlag[i]->setStyleSheet("background-color: red");
                    break;
                case 0:
                   BatteryStatusBitFlag[i]->setStyleSheet("background-color: green");
                   break;
                }
            }
            else BatteryStatusBitFlag[i]->setStyleSheet("default");
        }

        HidSmbus_Close(m_hidSmbus);
//        HidSmbus_Reset(m_hidSmbus);
    }
}



void MainWindow::on_pushButton_2_clicked()
{
//    CP2112_Open1(DevNumber);
    BOOL                    opened;         //Открылось ли устройство
    HID_SMBUS_STATUS        status;

    status = HidSmbus_Open(&m_hidSmbus, DevNumber, VID, PID);
//    if (status != HID_SMBUS_SUCCESS) return FALSE;
    status = HidSmbus_IsOpened(m_hidSmbus, &opened);
//    if (status != HID_SMBUS_SUCCESS) return FALSE;
//    if (!opened)return FALSE;
//    return TRUE;   // Успех!

}

