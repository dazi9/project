// 酒店管理系统主程序
// Google C++ Style，含详细注释
#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>

using namespace std;

void SetConsole()
{
	// 设置控制台编码为UTF-8以支持中文显示
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
};

// 房间结构体
struct Room
{
	int room_id;   // 房间号
	char type[30]; // 房间类型
	double price;  // 房间价格
	int status;	   // 0无人 1有人
	Room *next;	   // 下一个房间指针
};

// 客人结构体
struct Guest
{
	char id[20];	   // 客人身份证号
	char name[50];	   // 客人姓名
	char checkin[20];  // 入住时间
	char checkout[20]; // 离开时间
	int room_id;	   // 入住房间号
	double cost;	   // 总花费
	Guest *next;	   // 下一个客人指针
};

// 全局链表头指针
Room *room_head = nullptr;
Guest *guest_head = nullptr;

// 加载房间信息
// 文件格式：房间编号,类型,价格,状态
void LoadRooms()
{
	std::ifstream fin("room.txt");
	if (!fin)
		return;
	Room *tail = nullptr;
	while (!fin.eof())
	{
		Room *r = new Room;
		char line[128];
		if (!fin.getline(line, 128))
			break;
		if (strlen(line) == 0)
		{
			delete r;
			break;
		}
		sscanf(line, "%d,%29[^,],%lf,%d", &r->room_id, r->type, &r->price, &r->status);
		r->next = nullptr;
		if (!room_head)
			room_head = tail = r;
		else
		{
			tail->next = r;
			tail = r;
		}
	}
	fin.close();
}

// 保存房间信息
void SaveRooms()
{
	std::ofstream fout("room.txt");
	for (Room *p = room_head; p; p = p->next)
	{
		fout << p->room_id << "," << p->type << "," << p->price << "," << p->status << std::endl;
	}
	fout.close();
}

// 加载客人信息
// 文件格式：身份证号,姓名,入住时间,离开时间,房间编号,总花费
void LoadGuests()
{
	std::ifstream fin("guest.txt");
	if (!fin)
		return;
	Guest *tail = nullptr;
	while (!fin.eof())
	{
		Guest *g = new Guest;
		char line[256];
		if (!fin.getline(line, 256))
			break;
		if (strlen(line) == 0)
		{
			delete g;
			break;
		}
		sscanf(line, "%19[^,],%49[^,],%19[^,],%19[^,],%d,%lf", g->id, g->name, g->checkin, g->checkout, &g->room_id, &g->cost);
		g->next = nullptr;
		if (!guest_head)
			guest_head = tail = g;
		else
		{
			tail->next = g;
			tail = g;
		}
	}
	fin.close();
}

// 保存客人信息
void SaveGuests()
{
	std::ofstream fout("guest.txt");
	for (Guest *p = guest_head; p; p = p->next)
	{
		fout << p->id << "," << p->name << "," << p->checkin << "," << p->checkout << "," << p->room_id << "," << p->cost << std::endl;
	}
	fout.close();
}

// ...后续所有代码全部缩进为4个空格...
void AddRoom()
{
	int room_id;
	std::cout << "请输入房间号: ";
	std::cin >> room_id;
	// 检查房间号是否已存在
	for (Room *p = room_head; p; p = p->next)
	{
		if (p->room_id == room_id)
		{
			std::cout << "该房间号已存在，添加失败！" << std::endl;
			return;
		}
	}
	Room *r = new Room;
	r->room_id = room_id;
	std::cout << "请输入房间类型: ";
	std::cin >> r->type;
	std::cout << "请输入房间价格: ";
	std::cin >> r->price;
	r->status = 0;
	r->next = room_head;
	room_head = r;
	SaveRooms();
	std::cout << "添加成功！" << std::endl;
}

// 查询房间
void QueryRoom()
{
	int id;
	std::cout << "请输入要查询的房间号: ";
	std::cin >> id;
	for (Room *p = room_head; p; p = p->next)
	{
		if (p->room_id == id)
		{
			std::cout << "房间号: " << p->room_id << ", 类型: " << p->type << ", 价格: " << p->price << ", 状态: " << (p->status ? "有人" : "无人") << std::endl;
			return;
		}
	}
	std::cout << "未找到该房间！" << std::endl;
}

// 预订房间（新增客人信息）
void BookRoom()
{
	int id;
	char name[50], idcard[20], checkin[20], checkout[20];
	std::cout << "请输入要预订的房间号: ";
	std::cin >> id;
	for (Room *p = room_head; p; p = p->next)
	{
		if (p->room_id == id && p->status == 0)
		{
			std::cout << "请输入客人姓名: ";
			std::cin >> name;
			std::cout << "请输入客人身份证号: ";
			std::cin >> idcard;
			std::cout << "请输入入住时间(如2025-12-28): ";
			std::cin >> checkin;
			std::cout << "请输入离开时间(如2025-12-30): ";
			std::cin >> checkout;
			p->status = 1;
			Guest *g = new Guest;
			strcpy(g->name, name);
			strcpy(g->id, idcard);
			strcpy(g->checkin, checkin);
			strcpy(g->checkout, checkout);
			g->room_id = id;
			// 计算花费（简单处理：离开-入住天数*房价）
			g->cost = p->price; // 可扩展为天数*房价
			g->next = guest_head;
			guest_head = g;
			SaveRooms();
			SaveGuests();
			std::cout << "预订成功！" << std::endl;
			return;
		}
	}
	std::cout << "房间不可用或不存在！" << std::endl;
}
// 查询客人信息（按姓名或身份证号）
void QueryGuest()
{
	int op;
	std::cout << "1. 按姓名查询 2. 按身份证号查询 3. 显示全部客人信息\n请选择: ";
	std::cin >> op;
	if (op == 1)
	{
		char name[50];
		std::cout << "请输入客人姓名: ";
		std::cin >> name;
		bool found = false;
		for (Guest *g = guest_head; g; g = g->next)
		{
			if (strcmp(g->name, name) == 0)
			{
				std::cout << "姓名: " << g->name << ", 身份证号: " << g->id << ", 入住: " << g->checkin << ", 离开: " << g->checkout << ", 房间号: " << g->room_id << ", 花费: " << g->cost << std::endl;
				found = true;
			}
		}
		if (!found)
			std::cout << "未找到该客人！" << std::endl;
	}
	else if (op == 2)
	{
		char idcard[20];
		std::cout << "请输入身份证号: ";
		std::cin >> idcard;
		bool found = false;
		for (Guest *g = guest_head; g; g = g->next)
		{
			if (strcmp(g->id, idcard) == 0)
			{
				std::cout << "姓名: " << g->name << ", 身份证号: " << g->id << ", 入住: " << g->checkin << ", 离开: " << g->checkout << ", 房间号: " << g->room_id << ", 花费: " << g->cost << std::endl;
				found = true;
			}
		}
		if (!found)
			std::cout << "未找到该客人！" << std::endl;
	}
	else if (op == 3)
	{
		double total = 0;
		int cnt = 0;
		for (Guest *g = guest_head; g; g = g->next)
		{
			std::cout << "姓名: " << g->name << ", 身份证号: " << g->id << ", 入住: " << g->checkin << ", 离开: " << g->checkout << ", 房间号: " << g->room_id << ", 花费: " << g->cost << std::endl;
			total += g->cost;
			cnt++;
		}
		std::cout << "共 " << cnt << " 位客人，总花费: " << total << " 元" << std::endl;
	}
}

// 新增客人信息（不预订房间）
void AddGuest()
{
	char name[50], idcard[20], checkin[20], checkout[20];
	int room_id;
	std::cout << "请输入客人姓名: ";
	std::cin >> name;
	std::cout << "请输入身份证号: ";
	std::cin >> idcard;
	// 检查身份证号是否已存在
	for (Guest *g = guest_head; g; g = g->next)
	{
		if (strcmp(g->id, idcard) == 0)
		{
			std::cout << "该身份证号已存在，添加失败！" << std::endl;
			return;
		}
	}
	std::cout << "请输入房间号: ";
	std::cin >> room_id;
	std::cout << "请输入入住时间(如2025-12-28): ";
	std::cin >> checkin;
	std::cout << "请输入离开时间(如2025-12-30): ";
	std::cin >> checkout;
	// 检查房间是否存在且空闲
	Room *rp = nullptr;
	for (Room *p = room_head; p; p = p->next)
	{
		if (p->room_id == room_id && p->status == 0)
		{
			rp = p;
			break;
		}
	}
	if (!rp)
	{
		std::cout << "房间不存在或已被占用！" << std::endl;
		return;
	}
	rp->status = 1;
	Guest *g = new Guest;
	strcpy(g->name, name);
	strcpy(g->id, idcard);
	strcpy(g->checkin, checkin);
	strcpy(g->checkout, checkout);
	g->room_id = room_id;
	g->cost = rp->price; // 可扩展为天数*房价
	g->next = guest_head;
	guest_head = g;
	SaveRooms();
	SaveGuests();
	std::cout << "添加客人成功！" << std::endl;
}

// 删除客人信息
void DeleteGuest()
{
	char idcard[20];
	std::cout << "请输入要删除的客人身份证号: ";
	std::cin >> idcard;
	Guest *prev = nullptr;
	for (Guest *g = guest_head; g; prev = g, g = g->next)
	{
		if (strcmp(g->id, idcard) == 0)
		{
			// 释放房间
			for (Room *r = room_head; r; r = r->next)
			{
				if (r->room_id == g->room_id)
				{
					r->status = 0;
					break;
				}
			}
			if (prev)
				prev->next = g->next;
			else
				guest_head = g->next;
			delete g;
			SaveRooms();
			SaveGuests();
			std::cout << "删除成功！" << std::endl;
			return;
		}
	}
	std::cout << "未找到该客人！" << std::endl;
}

// 修改客人信息
void ModifyGuest()
{
	char idcard[20];
	std::cout << "请输入要修改的客人身份证号: ";
	std::cin >> idcard;
	for (Guest *g = guest_head; g; g = g->next)
	{
		if (strcmp(g->id, idcard) == 0)
		{
			std::cout << "原姓名: " << g->name << ", 请输入新姓名: ";
			std::cin >> g->name;
			std::cout << "原房间号: " << g->room_id << ", 请输入新房间号: ";
			int new_room;
			std::cin >> new_room;
			// 检查新房间是否可用
			Room *rp = nullptr;
			for (Room *r = room_head; r; r = r->next)
			{
				if (r->room_id == new_room && r->status == 0)
				{
					rp = r;
					break;
				}
			}
			if (!rp)
			{
				std::cout << "新房间不存在或已被占用！" << std::endl;
				return;
			}
			// 释放原房间
			for (Room *r = room_head; r; r = r->next)
			{
				if (r->room_id == g->room_id)
				{
					r->status = 0;
					break;
				}
			}
			// 占用新房间
			rp->status = 1;
			g->room_id = new_room;
			std::cout << "原入住时间: " << g->checkin << ", 请输入新入住时间: ";
			std::cin >> g->checkin;
			std::cout << "原离开时间: " << g->checkout << ", 请输入新离开时间: ";
			std::cin >> g->checkout;
			g->cost = rp->price; // 可扩展为天数*房价
			SaveRooms();
			SaveGuests();
			std::cout << "修改成功！" << std::endl;
			return;
		}
	}
	std::cout << "未找到该客人！" << std::endl;
}

// 退房
void CheckoutRoom()
{
	int id;
	std::cout << "请输入要退房的房间号: ";
	std::cin >> id;
	for (Room *p = room_head; p; p = p->next)
	{
		if (p->room_id == id && p->status == 1)
		{
			p->status = 0;
			// 从客人链表删除
			Guest *prev = nullptr;
			for (Guest *g = guest_head; g; prev = g, g = g->next)
			{
				if (g->room_id == id)
				{
					if (prev)
						prev->next = g->next;
					else
						guest_head = g->next;
					delete g;
					break;
				}
			}
			SaveRooms();
			SaveGuests();
			std::cout << "退房成功！" << std::endl;
			return;
		}
	}
	std::cout << "房间未入住或不存在！" << std::endl;
}

// 显示所有房间
void ShowAllRooms()
{
	std::cout << "房间列表：" << std::endl;
	for (Room *p = room_head; p; p = p->next)
	{
		std::cout << "房间号: " << p->room_id << ", 类型: " << p->type << ", 价格: " << p->price << ", 状态: " << (p->status ? "有人" : "无人") << std::endl;
	}
}

// 主菜单
// 一级主菜单
void MainMenu()
{
	std::cout << "\n====== 酒店管理系统 ======" << std::endl;
	std::cout << "1. 客房相关" << std::endl;
	std::cout << "2. 客人相关" << std::endl;
	std::cout << "0. 退出系统" << std::endl;
	std::cout << "请选择操作: ";
}

// 客房相关二级菜单
void RoomMenu()
{
	std::cout << "\n--- 客房管理 ---" << std::endl;
	std::cout << "1. 添加房间" << std::endl;
	std::cout << "2. 查询房间" << std::endl;
	std::cout << "3. 预订房间" << std::endl;
	std::cout << "4. 退房" << std::endl;
	std::cout << "5. 显示所有房间" << std::endl;
	std::cout << "0. 返回主菜单" << std::endl;
	std::cout << "请选择操作: ";
}

// 客人相关二级菜单
void GuestMenu()
{
	std::cout << "\n--- 客人管理 ---" << std::endl;
	std::cout << "1. 查询客人信息" << std::endl;
	std::cout << "2. 新增客人信息" << std::endl;
	std::cout << "3. 删除客人信息" << std::endl;
	std::cout << "4. 修改客人信息" << std::endl;
	std::cout << "0. 返回主菜单" << std::endl;
	std::cout << "请选择操作: ";
}


void UI()
{
	int main_choice;
	do
	{
		MainMenu();
		std::cin >> main_choice;
		switch (main_choice)
		{
		case 1:
		{ // 客房相关
			int room_choice;
			do
			{
				RoomMenu();
				std::cin >> room_choice;
				switch (room_choice)
				{
				case 1:
					AddRoom();
					break;
				case 2:
					QueryRoom();
					break;
				case 3:
					BookRoom();
					break;
				case 4:
					CheckoutRoom();
					break;
				case 5:
					ShowAllRooms();
					break;
				case 0:
					std::cout << "返回主菜单..." << std::endl;
					break;
				default:
					std::cout << "无效选择，请重试。" << std::endl;
				}
			} while (room_choice != 0);
			break;
		}
		case 2:
		{ // 客人相关
			int guest_choice;
			do
			{
				GuestMenu();
				std::cin >> guest_choice;
				switch (guest_choice)
				{
				case 1:
					QueryGuest();
					break;
				case 2:
					AddGuest();
					break;
				case 3:
					DeleteGuest();
					break;
				case 4:
					ModifyGuest();
					break;
				case 0:
					std::cout << "返回主菜单..." << std::endl;
					break;
				default:
					std::cout << "无效选择，请重试。" << std::endl;
				}
			} while (guest_choice != 0);
			break;
		}
		case 0:
			SaveRooms();
			SaveGuests();
			std::cout << "已退出。" << std::endl;
			break;
		default:
			std::cout << "无效选择，请重试。" << std::endl;
		}
	} while (main_choice != 0);
}

int main()
{
	SetConsole();
	LoadRooms();
	LoadGuests();
	UI();
	return 0;
}
