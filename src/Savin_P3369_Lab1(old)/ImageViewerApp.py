import tkinter as tk
from tkinter import filedialog, messagebox
from PIL import Image, ImageTk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np
import os


class ImageViewerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Программа по работе с фото")
        self.root.geometry("1200x800")
        
        # Переменные для хранения изображений
        self.current_image = None
        self.original_image = None
        self.photo_image = None
        self.default_images = []  # Список заготовленных изображений
        self.current_default_index = 0  # Текущий индекс заготовленного изображения
        
        # Создание интерфейса
        self.create_widgets()
        
        # Создаем заготовленные изображения
        self.create_default_images()
        # Загрузка начального изображения
        self.load_default_image()

    def create_widgets(self):
        # Основной фрейм
        main_frame = tk.Frame(self.root)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # Фрейм для кнопок
        button_frame = tk.Frame(main_frame)
        button_frame.pack(fill=tk.X, pady=5)
        
        # Кнопки
        self.load_btn = tk.Button(button_frame, text="Загрузить изображение", 
                                    command=self.load_image, bg="lightblue")
        self.load_btn.pack(side=tk.LEFT, padx=5)
        
        self.next_default_btn = tk.Button(button_frame, text="Следующее изображение", 
                                         command=self.next_default_image, bg="lightyellow")
        self.next_default_btn.pack(side=tk.LEFT, padx=5)

        # Фрейм для изображения и гистограммы
        content_frame = tk.Frame(main_frame)
        content_frame.pack(fill=tk.BOTH, expand=True)
        
        # Фрейм для изображения
        self.image_frame = tk.Frame(content_frame, relief=tk.SUNKEN, bd=2)
        self.image_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(0, 5))
        
        # Метка для изображения
        self.image_label = tk.Label(self.image_frame, text="Изображение будет здесь", 
                                    bg="white", relief=tk.SUNKEN, bd=1)
        self.image_label.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Фрейм для гистограммы
        self.hist_frame = tk.Frame(content_frame, relief=tk.SUNKEN, bd=2)
        self.hist_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=(5, 0))
        
        # Метка для гистограммы
        self.hist_label = tk.Label(self.hist_frame, text="Гистограмма будет здесь", 
                                    bg="white", relief=tk.SUNKEN, bd=1)
        self.hist_label.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Фрейм для информации о пикселях
        info_frame = tk.Frame(main_frame)
        info_frame.pack(fill=tk.X, pady=5)
        
        self.info_label = tk.Label(info_frame, text="Информация о изображении: ", 
                                    bg="lightgray", relief=tk.RAISED, bd=1)
        self.info_label.pack(fill=tk.X, padx=5, pady=2)
        
        # Привязка события движения мыши для отображения информации о пикселе
        self.image_label.bind("<Motion>", self.show_pixel_info)
        self.image_label.bind("<Leave>", self.clear_pixel_info)

    def create_default_images(self):
        """Загрузка заготовленных изображений из папки"""
        # Папка Images в той же директории, что и скрипт
        script_dir = os.path.dirname(os.path.abspath(__file__))
        folder_path = os.path.join(script_dir, "Images")
        self.default_images = []
        
        try:
            # Проверяем существование папки
            if not os.path.exists(folder_path):
                print(f"Папка {folder_path} не найдена!")
                # Создаем папку, если её нет
                os.makedirs(folder_path)
                print(f"Создана папка {folder_path}. Добавьте туда изображения.")
                return
            
            # Получаем список всех файлов изображений в папке
            image_extensions = ('.jpg', '.jpeg', '.png', '.bmp', '.gif', '.tiff')
            image_files = []
            
            for file in os.listdir(folder_path):
                if file.lower().endswith(image_extensions):
                    image_files.append(os.path.join(folder_path, file))
            
            # Сортируем файлы
            image_files.sort()
            
            if not image_files:
                print(f"В папке {folder_path} не найдено изображений!")
                return
            
            # Загружаем изображения
            for image_path in image_files:
                try:
                    image = Image.open(image_path)
                    # Конвертируем в RGB если нужно
                    if image.mode != 'RGB':
                        image = image.convert('RGB')
                    self.default_images.append(image)
                    print(f"Загружено изображение: {os.path.basename(image_path)}")
                except Exception as e:
                    print(f"Ошибка загрузки {image_path}: {e}")
            
            print(f"Всего загружено {len(self.default_images)} изображений")
                
        except Exception as e:
            print(f"Ошибка доступа к папке: {e}")

        
    def load_default_image(self):
        """Загрузка стандартного изображения"""
        if self.default_images:
            self.current_image = self.default_images[self.current_default_index]
            self.original_image = self.current_image.copy()
            self.display_image()

    def next_default_image(self):
        """Переключение на следующее заготовленное изображение"""
        if self.default_images:
            self.current_default_index = (self.current_default_index + 1) % len(self.default_images)
            self.load_default_image()

    def load_image(self):
        """Загрузка изображения с компьютера"""
        file_path = filedialog.askopenfilename(
            title="Выберите изображение",
            filetypes=[("Image files", "*.jpg *.jpeg *.png *.bmp *.gif *.tiff")]
        )
        
        if file_path:
            try:
                image = Image.open(file_path)
                self.current_image = image
                self.original_image = image.copy()
                self.display_image()
            except Exception as e:
                messagebox.showerror("Ошибка", f"Не удалось загрузить изображение: {str(e)}")

    def display_image(self):
        """Отображение текущего изображения"""
        if self.current_image:
            # Масштабируем изображение для отображения
            display_size = (600, 400)
            image = self.current_image.copy()
            image.thumbnail(display_size, Image.Resampling.LANCZOS)
            
            self.photo_image = ImageTk.PhotoImage(image)
            self.image_label.configure(image=self.photo_image, text="")
            
            # Обновляем информацию об изображении
            info_text = f"Размер: {self.current_image.size} | Формат: {self.current_image.mode}"
            self.info_label.configure(text=info_text)

            self.show_histogram()

    def show_histogram(self):
        """Отображение гистограммы RGB"""
        if self.current_image:
            # Создаем фигуру matplotlib
            fig, ax = plt.subplots(figsize=(6, 4))
            
            # Конвертируем изображение в numpy array
            img_array = np.array(self.current_image)
            
            # Разделяем каналы
            if len(img_array.shape) == 3:  # RGB изображение
                colors = ['red', 'green', 'blue']
                for i, color in enumerate(colors):
                    channel = img_array[:, :, i].flatten()
                    ax.hist(channel, bins=50, alpha=0.7, color=color, label=color.upper())
            else:  # Grayscale изображение
                channel = img_array.flatten()
                ax.hist(channel, bins=50, alpha=0.7, color='gray', label='Grayscale')
            
            ax.set_title('Гистограмма цветов')
            ax.set_xlabel('Значение пикселя')
            ax.set_ylabel('Количество')
            ax.legend()
            ax.grid(True, alpha=0.3)
            
            # Встраиваем гистограмму в Tkinter
            for widget in self.hist_frame.winfo_children():
                widget.destroy()
            
            canvas = FigureCanvasTkAgg(fig, self.hist_frame)
            canvas.draw()
            canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

    def show_pixel_info(self, event):
        """Показ информации о пикселе под курсором мыши"""
        if self.current_image and self.photo_image:
            # Получаем координаты относительно изображения
            x = event.x
            y = event.y
            
            # Масштабируем координаты к оригинальному размеру изображения
            display_width = self.photo_image.width()
            display_height = self.photo_image.height()
            
            if (0 <= x < display_width and 0 <= y < display_height):
                scale_x = self.current_image.width / display_width
                scale_y = self.current_image.height / display_height
                
                orig_x = int(x * scale_x)
                orig_y = int(y * scale_y)
                
                # Получаем цвет пикселя
                if orig_x < self.current_image.width and orig_y < self.current_image.height:
                    pixel = self.current_image.getpixel((orig_x, orig_y))
                    
                    if isinstance(pixel, int):  # Grayscale
                        info = f"X: {orig_x}, Y: {orig_y} | Gray: {pixel}"
                    else:  # RGB
                        info = f"X: {orig_x}, Y: {orig_y} | RGB: {pixel}"
                    
                    self.info_label.configure(text=info)

    def clear_pixel_info(self, event):
        """Очистка информации о пикселе"""
        if self.current_image:
            info_text = f"Размер: {self.current_image.size} | Формат: {self.current_image.mode}"
            self.info_label.configure(text=info_text)


