//												12/9/2024-PĐ
/*Pac-Man (パックマン Pakkuman) là một trò chơi arcade được phát triển bởi Namco
 *Phát hành đầu tiên tại Nhật Bản vào 22 tháng 5 năm 1980.
 *Pac-Man là trò chơi điện tử được biết đến nhiều nhất đối với những người chơi Hoa Kỳ, trong đó con số trong cuộc khảo sát là 94%
 *Tên gọi ban đầu của trò chơi này là "Puck-Man" nhưng rồi khi phát hành sang Mỹ, tên được đổi lại thành "Pac-Man" để tránh bị sửa đổi thành một từ thô tục. */

//Pac-Man (パックマン Pakkuman) is an arcade game developed by Namco.
//---------------------------******--------------------------------------------------------------------------------------------------------------------------------------
//It was first released in Japan on May 22, 1980.
//Pac-Man is the most well-known video game among American players, with 94% of respondents recognizing it in a survey.
//The original name of the game was "Puck-Man," but it was changed to "Pac-Man" when released in the United States to prevent it from being altered into a vulgar term.


//Note!!!!
/*Nếu Các Bạn có xem qua dự án tôi trên github thì hãy đừng quên cho tôi một bình luận và yêu thích ,
 * tuy dự án này không phải tôi tự viết hết , tôi đã phải dựa vào những gợi ý là những thư viện có sẵn để giúp tôi hoàn thành , mong mọi người nếu có đọc được thì hãy thông cảm cho tôi vì những sự sai sót nếu các bạn thấy trong dự án tôi gửi,
 *  tôi mong nếu khoảng vài năm nữa tôi sẽ cố gắng kiếm thêm những dự án hay ho để đăng lên cho các bạn cùng xem .*/

//If you happen to check out my project on GitHub, please don't forget to leave me a comment and a like. Although I didn't write the entire project from scratch,
//I relied on suggestions and available libraries to help me complete it. I hope you can understand and be forgiving of any mistakes you might find in the project I shared. I aim to create and upload more exciting projects for everyone to see in the coming years.
//In this project, I only used the SSD1306 with the STM32F103C8T6 and four buttons connected to the PA pins for control.

//At first, I planned to create Super Mario, but after a few days, I reconsidered.
//I realized that when I tried to combine images within a single frame, it sometimes resulted in distortion or errors.
//So, I thought, why not aim for something simpler, like the frame structure of the classic Nokia Bounce game? I decided to use Pac-Man as the character.
//As a result, I combined these ideas and named it Super Pac-ManB.

//Here is the YouTube channel I referenced to develop my project:
//https://www.youtube.com/watch?v=iG-aBJrOFpU
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "fonts.h"
#include "ssd1306_oled.h"
#include "bmp.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>  // Để sử dụng hàm rand()
#include <stdbool.h>

#define MAX_PELLETS 2
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define TILE_WIDTH    10// Mỗi ô rộng 20 pixel
#define TILE_HEIGHT   6 // Mỗi ô cao 8 pixel
#define MAP_WIDTH 18   // Chiều rộng bản đồ (số ô)
#define MAP_HEIGHT 32  // Chiều cao bản đồ (số ô)
#define PACMAN_RADIUS (TILE_WIDTH / 2 - 1)  // Đặt bán kính Pac-Man phù hợp với TILE_WIDTH và TILE_HEIGHT


int pellet_count = MAX_PELLETS;
int pacman_x = 1;  // Vị trí ban đầu của Pac-Man (x)
int pacman_y = 1;  // Vị trí ban đầu của Pac-Man (y)
int pellets[MAX_PELLETS][1];
char buf_oled[20];

// Hàm tạo các Pellet ngẫu nhiên trong phạm vi bản đồ
void DrawPellet(int pellet_x, int pellet_y) {
    int radius = 2;
    int x = pellet_x * TILE_WIDTH + TILE_WIDTH / 2;
    int y = pellet_y * TILE_HEIGHT + TILE_HEIGHT / 2;

    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        SSD1306_DrawFilledCircle(x, y, radius, WHITE);
    }
}

void move_pacman(int offset_x, int offset_y) {

    pacman_x += offset_x;
    pacman_y += offset_y;

    for (int i = 0; i < MAX_PELLETS; i++) {
        if (pellets[i][0] != -1 && pellets[i][1] != -1) {
            pellets[i][0] += offset_x;
            pellets[i][1] += offset_y;

            // Đảm bảo Pellet không ra ngoài phạm vi màn hình
            if (pellets[i][0] < 0) pellets[i][0] = 0;
            if (pellets[i][0] >= MAP_WIDTH) pellets[i][0] = MAP_WIDTH - 1;
            if (pellets[i][1] < 0) pellets[i][1] = 0;
            if (pellets[i][1] >= MAP_HEIGHT) pellets[i][1] = MAP_HEIGHT - 1;
        }
    }
}

void generate_pellets(void) {
    for (int i = 0; i < MAX_PELLETS; i++) {
        int x, y;
        bool valid = false;

        while (!valid) {
            x = rand() % MAP_WIDTH;
            y = rand() % MAP_HEIGHT;

            valid = true;
            for (int j = 0; j < i; j++) {
                if (pellets[j][0] == x && pellets[j][1] == y) {
                    valid = false;
                    break;
                }
            }
        }

        pellets[i][0] = x;
        pellets[i][1] = y;
    }
}

void DrawPellets() {
    for (int i = 0; i < MAX_PELLETS; i++) {
        if (pellets[i][0] != -1 && pellets[i][1] != -1) {  // Pellet chưa bị xóa
            DrawPellet(pellets[i][0], pellets[i][1]);
        }
    }
}

// Hàm kiểm tra va chạm với Pellet
int check_collision_with_pellet(int pacman_x, int pacman_y) {
    for (int i = 0; i < MAX_PELLETS; i++) {
        if (pellets[i][0] == pacman_x && pellets[i][1] == pacman_y) {
            return i;
        }
    }
    return -1;
}

void remove_pellet(int index) {
    pellets[index][0] = -1; // Đặt giá trị x và y của Pellet thành -1 để ẩn
    pellets[index][1] = -1;
}


// Hàm vẽ Pellet


void draw_pacman(int x, int y) {
    int centerX = x * TILE_WIDTH + TILE_WIDTH ;
    int centerY = y * TILE_HEIGHT + TILE_HEIGHT ;
    int radius = PACMAN_RADIUS;

    SSD1306_DrawFilledCircle(centerX, centerY, radius, WHITE);

    int start_angle = -45;
    int end_angle = 30;
    for (int angle = start_angle; angle <= end_angle; angle++) {
        int x1 = centerX + radius * cos(angle * M_PI / 180.0);
        int y1 = centerY + radius * sin(angle * M_PI / 180.0);
        SSD1306_DrawLine(centerX, centerY, x1, y1, BLACK);
    }
}

#define DEBOUNCE_DELAY 50  // Thời gian trễ chống rung (ms)
static uint32_t last_button_time = 0;  // Biến lưu trữ thời gian lần nhấn nút cuối cùng
void control_pacman(void) {
    uint32_t current_time = HAL_GetTick();

    if (current_time - last_button_time > DEBOUNCE_DELAY) {
        // Di chuyển Pac-Man theo các nút bấm
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET && pacman_y > 0) { // Lên
            pacman_y--;
        }
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET && pacman_y < (SCREEN_HEIGHT / TILE_HEIGHT) - 1) { // Xuống
            pacman_y++;
        }
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET && pacman_x > 0) { // Trái
            pacman_x--;
        }
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET && pacman_x < (SCREEN_WIDTH / TILE_WIDTH) - 1) { // Phải
            pacman_x++;
        }

        // Kiểm tra va chạm với Pellet và xóa Pellet nếu có va chạm
        int pellet_index = check_collision_with_pellet(pacman_x, pacman_y);
        if (pellet_index != -1) {
            remove_pellet(pellet_index);  // Pellet sẽ bị xóa ngay lập tức
        }

        last_button_time = current_time;
    }
}


void DrawFullScreenRoundedRectangle() {
    // Vẽ hình chữ nhật bo góc bao quanh toàn bộ màn hình SSD1306
    int x = 0;       // Tọa độ x của góc trên bên trái
    int y = 0;       // Tọa độ y của góc trên bên trái
    int width = 128; // Chiều rộng của màn hình SSD1306
    int height = 64; // Chiều cao của màn hình SSD1306
    int radius = 1;  // Bán kính góc bo
    SSD1306_COLOR_t color = WHITE; // Màu trắng

    // Gọi hàm vẽ hình chữ nhật bo góc
    DrawRoundedRectangle(x, y, width, height, radius, color);
}

void DrawMap() {
    int cellWidth = 4;  // Chiều rộng mỗi ô (theo pixel)
    int cellHeight = 3; // Chiều cao mỗi ô (theo pixel)
    int color = WHITE;  // Màu sắc cho hình chữ nhật

    // Duyệt qua tất cả các phần tử trong map

    for (int row = 0; row < 18; row++) {
        for (int col = 0; col < 32; col++) {
            // Nếu giá trị trong mảng khác 0, vẽ hình chữ nhật
            if (map[row][col] != 0) {
                int x = col * cellWidth;        // Tọa độ x của ô
                int y = row * cellHeight;       // Tọa độ y của ô
                int width = cellWidth;          // Chiều rộng của ô
                int height = cellHeight;        // Chiều cao của ô
                // Vẽ hình chữ nhật (đặt viền)
                SSD1306_DrawRectangle(x, y, width, height, color);

                // Tô màu bên trong hình chữ nhật
                // Sử dụng hàm FillRectangle nếu có (hoặc vẽ từng điểm nếu cần)
                for (int py = y + 1; py < y + height - 1; py++) {
                    for (int px = x + 1; px < x + width - 1; px++) {
                        SSD1306_DrawPixel(px, py, color);
                    }
                }
            }
        }
    }
}
// Hàm kiểm tra nếu còn Pellet nào
bool are_pellets_left() {
    for (int i = 0; i < MAX_PELLETS; i++) {
        if (pellets[i][0] != -1 && pellets[i][1] != -1) {
            return true;  // Còn Pellet
        }
    }
    return false;  // Không còn Pellet nào
}
void display_win_message(void) {
    SSD1306_Fill(BLACK);  // Xóa màn hình
    SSD1306_GotoXY(20, 30);  // Đặt vị trí hiển thị
    SSD1306_Puts("YOU WIN", &Font_11x18, WHITE);  // Hiển thị thông báo "YOU WIN"
    SSD1306_UpdateScreen();  // Cập nhật màn hình
    HAL_Delay(0XFFFFFFFF);
}

void draw_game(void) {
    // Làm sạch màn hình một lần duy nhất
    SSD1306_Fill(BLACK);

    // Vẽ các Pellet chưa bị ăn
    DrawPellets();

    // Vẽ lại Pac-Man
    draw_pacman(pacman_x, pacman_y);

    // Vẽ lại bản đồ
    DrawMap();

    // Vẽ khung hình bo góc xung quanh màn hình
    DrawFullScreenRoundedRectangle();

    // Cập nhật màn hình chỉ một lần sau khi vẽ xong tất cả
    SSD1306_UpdateScreen();
}


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Hàm vẽ bản đồ lên màn hình

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	HAL_Init();
	    SystemClock_Config();
	    MX_GPIO_Init();
	    MX_I2C1_Init();

	    SSD1306_Init();
		generate_pellets();
		SSD1306_GotoXY(5,10);
		SSD1306_Puts("Pac-Man", &Font_11x18, WHITE);
		SSD1306_GotoXY(10,30);
		SSD1306_Puts("Welcome", &Font_16x26, WHITE);
		SSD1306_UpdateScreen();
		HAL_Delay(2000);

		SSD1306_Clear();
		SSD1306_GotoXY(40,10);
		SSD1306_Puts("Note!!!\n", &Font_7x10, WHITE);
		SSD1306_GotoXY(25,25);
		SSD1306_Puts("Super Pac-Man", &Font_7x10, WHITE);
		SSD1306_GotoXY(13,35);
		SSD1306_Puts("Namco & Midway", &Font_7x10, WHITE);
		SSD1306_GotoXY(33,50);
		SSD1306_Puts("12-2-24",&Font_7x10,WHITE);
		SSD1306_UpdateScreen();
		HAL_Delay(4000);

		SSD1306_Clear();
		SSD1306_DrawRectangle(2, 2, 123, 60, WHITE); // Khung bao quanh
		SSD1306_GotoXY(10, 10);
		SSD1306_Puts("Game Loading ^-^", &Font_7x10, WHITE); // Thay đổi thông báo
		SSD1306_UpdateScreen();

		uint8_t progress = 0;

			// Vẽ thanh tiến độ (màu trắng) phía dưới
		SSD1306_DrawRectangle(10, 45, 100, 10, WHITE);  // Khung thanh tiến độ
		SSD1306_UpdateScreen();

			// Thêm hiệu ứng cho thanh tiến độ (sử dụng màu đen làm nền và màu trắng làm thanh tiến độ)
			while (progress <= 100) {
				// Xóa thanh tiến độ trước khi vẽ lại
				SSD1306_DrawRectangle(10, 45, 100, 10, BLACK); // Xóa thanh tiến độ cũ (màu nền đen)
				SSD1306_DrawRectangle(10, 45, progress, 10, WHITE);  // Vẽ thanh tiến độ mới
				SSD1306_UpdateScreen();

				// Hiển thị phần trăm
				SSD1306_GotoXY(40, 30);
				sprintf(buf_oled, "%u%%", progress);
				SSD1306_Puts(buf_oled, &Font_11x18, WHITE);

				progress++; // Tăng phần trăm
				HAL_Delay(100);  // Delay mỗi lần cập nhật (100ms)
			}

			// Hoàn thành thanh loading (100%)
		SSD1306_DrawRectangle(10, 45, 100, 10, WHITE); // Đảm bảo thanh tiến độ đầy
		SSD1306_UpdateScreen();
		HAL_Delay(1000);  // Dừng lại một chút để người dùng thấy kết quả

		SSD1306_Clear();
		SSD1306_DrawBitmap(0, 0, imagen3, 128, 60, WHITE);
		SSD1306_UpdateScreen();
		HAL_Delay(2000);

		SSD1306_Clear();
		SSD1306_GotoXY(10,10);
		SSD1306_DrawRectangle(2, 2, 123, 60, WHITE);
		SSD1306_Puts("Click Get Started...", &Font_7x10, WHITE);
		SSD1306_GotoXY(48,38);
		SSD1306_Puts("^-^",&Font_7x10,WHITE);
		SSD1306_DrawCircle(60, 38, 16, WHITE);
		SSD1306_UpdateScreen();
		HAL_Delay(2000);

	  SSD1306_Clear();
	  SSD1306_DrawRectangle(2, 2, 123, 60, WHITE);

	  SSD1306_GotoXY(10, 10);
	  SSD1306_Puts("Getting Started..", &Font_7x10, WHITE);
	  SSD1306_DrawCircle(95, 35, 16, WHITE);
	  SSD1306_UpdateScreen();

	  HAL_Delay(1000); // Dừng lại để người dùng có thể đọc thông báo ban đầu.

	  // Khởi tạo biến để lưu tiến trình.
	  uint8_t progress1 = 0;

	    	  	  while (progress1 <= 100) {
	    	  	      // Vẽ lại giao diện
	    	  	      SSD1306_Clear();
	    	  	      SSD1306_DrawRectangle(2, 2, 123, 60, WHITE);
	    	  	      SSD1306_GotoXY(10, 10);
	    	  	      SSD1306_Puts("Loading...", &Font_7x10, WHITE);

	    	  	      // Vẽ thanh tiến trình.
	    	  	      SSD1306_DrawRectangle(10, 45, 100, 10, WHITE); // Khung của thanh.
	    	  	      SSD1306_DrawFilledRectangle(10, 45, progress1, 10, WHITE); // Thanh tiến trình thực tế.

	    	  	      // Hiển thị phần trăm.
	    	  	      char progressStr[5];
	    	  	      sprintf(progressStr, "%3d%%", progress1); // Định dạng phần trăm.
	    	  	      SSD1306_GotoXY(80, 30);
	    	  	      SSD1306_Puts(progressStr, &Font_7x10, WHITE);

	    	  	      // Cập nhật màn hình.
	    	  	      SSD1306_UpdateScreen();

	    	  	      // Tăng tiến trình và dừng một khoảng thời gian.
	    	  	      progress1++;
	    	  	      HAL_Delay(50); // Điều chỉnh thời gian để thanh tiến trình chạy chậm hoặc nhanh.
	    	  	  }

	  // Xóa màn hình và hiển thị thông báo "Let's Go".
	  SSD1306_Clear();
	  SSD1306_GotoXY(30, 25); // Căn giữa dòng chữ trên màn hình (tùy chỉnh vị trí phù hợp với kích thước màn hình).
	  SSD1306_Puts("Let's Go!", &Font_11x18, WHITE); // Dùng font lớn hơn để hiển thị rõ ràng.
	  SSD1306_UpdateScreen();
	  HAL_Delay(2000); // Giữ thông báo trên màn hình trong 2 giây.


	 SSD1306_Clear();
	 SSD1306_DrawBitmap(0, 0, imagen, 128, 60, WHITE);
	 SSD1306_UpdateScreen();
	 HAL_Delay(1000);

	 // Lặp qua từng ảnh để đảm bảo không có vùng đen
	 const uint8_t *images[] = { imagen5, imagen6};
	 for (int i = 0; i < 2; i++) {
		// SSD1306_ScrollRight(0, 0x0F);  // Kích hoạt scroll trước khi vẽ
		 SSD1306_Clear();  // Xóa toàn bộ để tránh vùng đen
		 SSD1306_DrawBitmap(0, 0, images[i], 128, 60, WHITE);
		 SSD1306_UpdateScreen();
		 HAL_Delay(3000);
	 }
	 SSD1306_Clear();
	 SSD1306_GotoXY(30, 25);
	 SSD1306_Puts("Go Go!", &Font_11x18, WHITE); // Dùng font lớn hơn để hiển thị rõ ràng.
	 SSD1306_UpdateScreen();
	 HAL_Delay(2000);
	 SSD1306_Stopscroll();
	 HAL_Delay(1000);
	 SSD1306_Clear();
	 srand(HAL_GetTick()); // Tạo seed ngẫu nhiên cho rand()
  while (1)
  {
    /* USER CODE END WHILE */
	  control_pacman();
	     draw_game();
	     // Kiểm tra va chạm với Pellet và xóa Pellet nếu có va chạm
	         int pellet_index = check_collision_with_pellet(pacman_x, pacman_y);
	         if (pellet_index != -1) {
	             remove_pellet(pellet_index);  // Pellet sẽ bị xóa ngay lập tức
	         }

	         // Vẽ lại màn hình
	         draw_game();

	         // Kiểm tra nếu không còn Pellet nào
	         if (!are_pellets_left()) {
	             display_win_message();  // Hiển thị thông báo "You Win"
	             HAL_Delay(3000);  // Delay 3 giây trước khi reset hoặc dừng game

	             // Reset lại game (hoặc dừng game)
	             generate_pellets(); // Tạo lại các pellet
	             pacman_x = 1; // Đặt lại vị trí Pac-Man
	             pacman_y = 1;
	         }

	         HAL_Delay(100); // Delay để điều chỉnh tốc độ vòng lặp



    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
