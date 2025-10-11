# Image Viewer Application

## English

### Description
A simple image viewer application built with Python and Tkinter for viewing images from a local folder.

### Features
- **Automatic image loading** from the "Images" folder
- **Two main functions**:
  - Load custom images from your computer
  - Navigate through images using "Следующее изображение" button
- **Circular navigation** - loops back to first image after last
- **Image information display** - shows current position, dimensions, and format
- **Automatic scaling** - maintains aspect ratio while fitting to display

### Supported Image Formats
- JPEG (.jpg, .jpeg)
- PNG (.png) 
- BMP (.bmp)
- GIF (.gif)

<p align="left">
  <img src="ForReadme/Example.png" width="500"/>
</p>

### Installation
1. Ensure Python 3.6+ is installed
2. Install required libraries:
```bash
pip install -r requirements.txt
```

Place images in "Images" folder

Run the application

### Usage
```bash
python main.py
```
### Buttons:
- "Загрузить изображение" - browse and load any image from computer
- "Следующее изображение" - cycle through images in the "Images" folder

### Requirements
- Python 3.6+
- Pillow library
- Matplotlib library
- Numpy library


## Русский

### Описание
Простое приложение для просмотра изображений, созданное на Python и Tkinter, для просмотра изображений из локальной папки.

### Функции
- **Автоматическая загрузка изображений** из папки «Изображения»
- **Две основные функции**:
    - Загрузка пользовательских изображений с компьютера
    - Навигация по изображениям с помощью кнопки «Следующее изображение»
- **Круговая навигация** — циклический переход к первому изображению после последнего
- **Отображение информации об изображении** — отображение текущего положения, размеров и формата
- **Автоматическое масштабирование** — сохранение соотношения сторон при подгонке изображения под размер экрана

### Поддерживаемые форматы изображений
- JPEG (.jpg, .jpeg)
- PNG (.png)
- BMP (.bmp)
- GIF (.gif)

<p align="left">
<img src="Images/Example.png" width="500"/>
</p>

### Установка
1. Убедитесь, что установлен Python 3.6+
2. Установите необходимые библиотеки:
```bash
pip install -r requirements.txt
```

Поместить изображения в папку "Images"

Запустить приложение

### Использование
```bash
python main.py
```
### Кнопки:
- "Загрузить изображение" - просмотреть и загрузить любое изображение с компьютера
- "Следующее изображение" - циклический переход по изображениям в папке "Images"

### Требования
- Python 3.6+
- Библиотека Pillow
- Библиотека Matplotlib
- Библиотека Numpy