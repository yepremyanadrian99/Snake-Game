#include <iostream>
#include <conio.h>
#include <thread>
#include <queue>
#include <ctime>
#include <chrono>
#include <Windows.h>
#include <vector>

int x, y, X, Y, score;
const int width = 20;
const int height = 10;  // for square - half the width

char movement_key;
int snake_direction = 1;
std::queue<int> queue_dirs;

using namespace std::chrono_literals;
const auto snake_speed = 0.15s;

bool GameOver = false;
bool wall_trespassing = true;
bool tail_trespassing = false;
bool use_colors = false;

std::vector<std::pair<int, int>> tail;

const int skin_id = 0;

// arrows
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
// WSAD
#define UP_CAPS 87
#define DOWN_CAPS 83
#define LEFT_CAPS 65
#define RIGHT_CAPS 68
// wsad
#define UP 119
#define DOWN 115
#define LEFT 97
#define RIGHT 100

void start();
void spawn_food();
void draw();
void control();
void move_snake(void);
void check_direction(void);
void add_to_tail();
bool check_collision();
bool eat_food();
void ShowConsoleCursor(bool);
WORD GetConsoleTextAttribute(HANDLE);
void set_color_red();
void set_color_green();
void set_color_blue();
void set_color_normal();

// colors!!!
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int saved_colors = GetConsoleTextAttribute(hConsole);
//

int main()
{
	system("timeout /t 2");
	ShowConsoleCursor(false);
	start();
}

void start()
{
	GameOver = false;
	x = width / 2;
	y = height / 2;
	/*tail.push_back(std::pair<int, int>(x, y + 1));
	tail.push_back(std::pair<int, int>(x, y + 2));
	tail.push_back(std::pair<int, int>(x, y + 3));
	tail.push_back(std::pair<int, int>(x, y + 4));
	tail.push_back(std::pair<int, int>(x, y + 5));
	tail.push_back(std::pair<int, int>(x, y + 6));
	tail.push_back(std::pair<int, int>(x, y + 7));
	tail.push_back(std::pair<int, int>(x, y + 8));
	tail.push_back(std::pair<int, int>(x, y + 9));
	tail.push_back(std::pair<int, int>(x, y + 10));
	tail.push_back(std::pair<int, int>(x, y + 11));
	tail.push_back(std::pair<int, int>(x, y + 12));
	tail.push_back(std::pair<int, int>(x, y + 13));
	tail.push_back(std::pair<int, int>(x, y + 14));
	tail.push_back(std::pair<int, int>(x, y + 15));
	tail.push_back(std::pair<int, int>(x, y + 16));
	tail.push_back(std::pair<int, int>(x, y + 17));
	tail.push_back(std::pair<int, int>(x, y + 18));
	tail.push_back(std::pair<int, int>(x, y + 19));
	tail.push_back(std::pair<int, int>(x, y + 20));
	tail.push_back(std::pair<int, int>(x, y + 21));
	tail.push_back(std::pair<int, int>(x, y + 22));
	tail.push_back(std::pair<int, int>(x, y + 23));
	tail.push_back(std::pair<int, int>(x, y + 24));
	tail.push_back(std::pair<int, int>(x, y + 25));
	tail.push_back(std::pair<int, int>(x, y + 26));
	tail.push_back(std::pair<int, int>(x, y + 27));
	tail.push_back(std::pair<int, int>(x, y + 28));
	tail.push_back(std::pair<int, int>(x, y + 29));
	tail.push_back(std::pair<int, int>(x, y + 30));*/
	score = 0;
	spawn_food();
	std::thread *th_move = new std::thread(move_snake);
	std::thread *th_check = new std::thread(check_direction);
	th_check->join();
	th_move->join();
}

void check_direction(void)
{
	while (!GameOver)
	{
		if (_kbhit())
		{
			movement_key = _getch();
			switch (movement_key)
			{
			case UP_ARROW:
			case UP_CAPS:
			case UP:
				if ((!queue_dirs.empty() && queue_dirs.front() != 1 && queue_dirs.front() != 2) ||
					snake_direction != 1 && snake_direction != 2)
					queue_dirs.push(1);
				break;
			case DOWN_ARROW:
			case DOWN_CAPS:
			case DOWN:
				if ((!queue_dirs.empty() && queue_dirs.front() != 1 && queue_dirs.front() != 2) ||
					snake_direction != 1 && snake_direction != 2)
					queue_dirs.push(2);
				break;
			case RIGHT_ARROW:
			case RIGHT_CAPS:
			case RIGHT:
				if ((!queue_dirs.empty() && queue_dirs.front() != 3 && queue_dirs.front() != 4) ||
					snake_direction != 3 && snake_direction != 4)
						queue_dirs.push(3);
				break;
			case LEFT_ARROW:
			case LEFT_CAPS:
			case LEFT:
				if ((!queue_dirs.empty() && queue_dirs.front() != 3 && queue_dirs.front() != 4) ||
					snake_direction != 3 && snake_direction != 4)
					queue_dirs.push(4);
				break;
			}
		}
	}
}

void draw()
{
	system("cls");
	control();
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			if (i == 0 || i == width - 1 || j == 0 || j == height - 1)
			{
				//set_color_red();
				std::cout << '#';
				//set_color_normal();
			}
			else if (i == x && j == y)
			{
				set_color_blue();
				switch (snake_direction)
				{
				case 1: //up
					std::cout << '^';
					break;
				case 2: //down
					std::cout << 'v';
					break;
				case 3: //right
					std::cout << '>';
					break;
				case 4: //left
					std::cout << '<';
					break;
				}
				set_color_normal();
			}
			else if (i == X && j == Y)
			{
				set_color_red();
				std::cout << '@';
				set_color_normal();
			}
			else
			{
				bool used = false;
				if (tail.size() > 0)
				{
					for (int k = 0; k < tail.size(); ++k)
					{
						if (i == tail[k].first && j == tail[k].second)
						{
							set_color_green();
							switch (skin_id)
							{
							case 0:
								std::cout << 'o';
								break;
							case 1:
								std::cout << char(65 + k);
								break;
							}
							used = true;
							set_color_normal();
							break;
						}
					}
				}
				if (!used)
					std::cout << ' ';
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Score: " << score << std::endl;
}

void control()
{
	if (!queue_dirs.empty())
	{
		snake_direction = queue_dirs.front();
		queue_dirs.pop();
	}
	switch (snake_direction)
	{
	case 1:
		--y;
		break;
	case 2:
		++y;
		break;
	case 3:
		++x;
		break;
	case 4:
		--x;
		break;
	}
	if (tail.size() >= (width - 2) * (height - 2) - 1)
	{
		//GameOver = true;
		std::cout << "YOU WON!!!!!" << std::endl;
		tail_trespassing = true;
	}
	if (check_collision())
		GameOver = true;
	if (!wall_trespassing && (x <= 1 || x > width - 2 || y <= 1 || y > height - 2))
		GameOver = true;
	if (x <= 0)
		x = width - 2;
	else if (x >= width - 1)
		x = 1;
	else if (y <= 0)
		y = height - 2;
	else if (y >= height - 1)
		y = 1;
	if (eat_food())
	{
		add_to_tail();
		spawn_food();
		score += 100;
	}
}

void move_snake(void)
{
	while (!GameOver)
	{
		if (tail.size() > 0)
		{
			for (int i = tail.size() - 1; i > 0; --i)
			{
				tail[i].first = tail[i - 1].first;
				tail[i].second = tail[i - 1].second;
			}
			tail[0].first = x;
			tail[0].second = y;
		}
		draw();
		std::this_thread::sleep_for(snake_speed);
	}
}

bool eat_food()
{
	if (X == x && Y == y)
		return true;
	for (int i = 0; i < tail.size(); ++i)
		if (tail[i].first == X && tail[i].second == Y)
			return true;
	return false;
}

void spawn_food()
{
	//X = rand() % (width - 2) + 1;
	//Y = rand() % (height - 2) + 1;
	bool ok = false;
	while (!ok)
	{
		ok = true;
		srand(time(NULL));
		X = rand() % (width - 2) + 1;
		Y = rand() % (height - 2) + 1;
		for (int i = 0; i < tail.size(); ++i)
			if (tail[i].first == X && tail[i].second == Y)
			{
				ok = false;
				break;
			}
	}
}

void add_to_tail()
{
	int dx = 0, dy = 0;
	if (snake_direction == 1)
		++dy;
	else if (snake_direction == 2)
		--dy;
	else if (snake_direction == 3)
		--dx;
	else
		++dx;
	if (tail.size() > 0)
		tail.push_back(std::pair<int, int>(tail[tail.size() - 1].first + dx, tail[tail.size() - 1].second + dy));
	else
		tail.push_back(std::pair<int, int>(x + dx, y + dy));
}

bool check_collision()
{
	if (tail_trespassing)
		return false;
	for (int i = 0; i < tail.size(); ++i)
		if (tail[i].first == x && tail[i].second == y)
			return true;
	return false;
}

void ShowConsoleCursor(bool show)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = show; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

WORD GetConsoleTextAttribute(HANDLE hConsole)
{
	CONSOLE_SCREEN_BUFFER_INFO con_info;
	GetConsoleScreenBufferInfo(hConsole, &con_info);
	return con_info.wAttributes;
}

void set_color_red()
{
	if (use_colors)
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void set_color_green()
{
	if (use_colors)
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
}

void set_color_blue()
{
	if (use_colors)
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

void set_color_normal()
{
	if (use_colors)
		SetConsoleTextAttribute(hConsole, saved_colors);
}