#include<iostream>
#include<time.h>
#include<Windows.h>

using namespace std;

// *�� �� �� :  ���� END_NUMBER��� * ���
const int END_NUMBER = 0;
// �÷��̾� ������
int playerBingo[25] = { 0, };
// AI ������
int aiBingo[25] = { 0, };

// ���� ���� ������ ������ �迭
int possibleBingo[25] = { 0, };
// ���� ���� ������ ������ ����
int possibleIndex = 0;

//�������� Ȯ��
// 0��° ���� 1��° END_NUMBER�� �ƴ� �� 2��° �÷��̾��� ����
int bingoXLine[5][3] = { 0, };
int bingoYLine[5][3] = { 0, };
int bingoDLine[2][3] = { 0, };

// AI �������� ����
int aiBingoCenter = 0;

// X��� Y�� �̵��� ����
int _countX = 0;
int _countY = 0;

// ��µ� ���ڵ��� �迭
int countArr[25] = { 0, };
int _count = 0;

// ��� ������ ���� enum
enum MODE {
	EASY = 1,
	NORMAL = 2,
	HARD = 3,
	HELL = 4,
	XLINEMOVE = 5,
	YLINEMOVE = 6
};

void baseMode(int mode);

/// <summary>
/// �������� ����
/// </summary>
/// <param name="arr"></param>
void setBingo(int arr[25]) {

	// �迭�� �ʱⰪ ����
	for (int i = 0; i < 25; i++) {
		arr[i] = i + 1;
	}

	// ������ �󸶳� ���� ����
	int count = rand() % 100 + 1;

	// ����
	for (int i = 0; i < count; i++)
	{
		int swapNum1 = rand() % 25;
		int swapNum2 = rand() % 25;

		int temp = arr[swapNum1];
		arr[swapNum1] = arr[swapNum2];
		arr[swapNum2] = temp;
	}
}

/// <summary>
/// ������ ���
/// </summary>
/// <param name="arr"></param>
void printBingo(int arr[25], int mode) {

	for (int i = 0; i < 25; i++) {

		if (i % 5 == 0) {
			cout << endl << endl;
		}

		if (arr[i] == END_NUMBER) {
			cout << "��";
		}
		else if (arr[i] < 10 && mode != HELL) {
			cout << 0 << arr[i];
		}
		else if (mode != HELL) {
			cout << arr[i];
		}
		else {
			cout << "��";
		}

		cout << "\t";
	}
	cout << endl << endl;
}

/// <summary>
/// X�� ���������� �̵�
/// </summary>
/// <param name="arr"></param>
void bingoXLineMove(int arr[25]) {

	if (_countX == 5) {
		_countX = 0;
	}

	int temp = arr[_countX * 5 + 4];
	for (int j = 4; j > 0; j--) {
		arr[_countX * 5 + j] = arr[_countX * 5 + j - 1];
	}
	arr[_countX * 5] = temp;
}

/// <summary>
/// Y�� �Ʒ��� �̵�
/// </summary>
/// <param name="arr"></param>
void bingoYLineMove(int arr[25]) {

	if (_countY == 5) {
		_countY = 0;
	}

	int temp = arr[5 * 4 + _countY];
	for (int j = 4; j > 0; j--) {
		arr[_countY + 5 * j] = arr[_countY + 5 * j - 5];
	}
	arr[_countY] = temp;
}

/// <summary>
/// �������� ���ڸ� END_NUMBER�� ��ȯ
/// </summary>
/// <param name="arr"></param>
void changeBingo(int arr[25], int selectNumber) {

	// ��ȯ
	for (int i = 0; i < 25; i++) {
		if (arr[i] == selectNumber) {
			arr[i] = END_NUMBER;
			break;
		}
	}
}

/// <summary>
/// ���� Ȯ��
/// </summary>
/// <param name="arr"></param>
/// <returns></returns>
int checkBingo(int arr[25]) {

	// ���� ����
	int bingo = 0;

	// ���� ���� �ʱ�ȭ
	for (int i = 0; i < 5; i++) {
		// ���� �� �ʱ�ȭ
		bingoXLine[i][0] = 0;

		// ���� �� �ʱ�ȭ
		bingoYLine[i][0] = 0;
		if (i < 2)
		{
			// �밢�� �ʱ�ȭ
			bingoDLine[i][0] = 0;
		}
	}

	// ���� ���� ���� Ȯ��
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (arr[i * 5 + j] == END_NUMBER) {
				bingoXLine[i][0] += 1;
			}
			else {
				bingoXLine[i][1] = arr[i * 5 + j];
			}
			if (arr[i + 5 * j] == END_NUMBER) {
				bingoYLine[i][0] += 1;
			}
			else {
				bingoYLine[i][1] = arr[i + 5 * j];
			}
			if (i == 2 && j == 2) {
				aiBingoCenter = arr[i * 5 + j];
			}
		}
	}

	// �밢�� ���� Ȯ��
	for (int i = 0; i <= 24; i += 6) {
		if (arr[i] == END_NUMBER) {
			bingoDLine[0][0] += 1;
		}
		else {
			bingoDLine[0][1] = arr[i];
		}
	}
	for (int i = 4; i <= 20; i += 4) {
		if (arr[i] == END_NUMBER) {
			bingoDLine[1][0] += 1;
		}
		else {
			bingoDLine[1][1] = arr[i];
		}
	}

	// ���� �� ����
	for (int i = 0; i < 5; i++) {
		if (bingoXLine[i][0] == 5) {
			bingo++;
		}
		if (bingoYLine[i][0] == 5) {
			bingo++;
		}
		if (i < 2) {
			if (bingoDLine[i][0] == 5) {
				bingo++;
			}
		}
	}

	return bingo;
}

/// <summary>
/// ����ȭ�� ���
/// </summary>
void startScreen() {
	system("cls");
	cout << "=============================================" << endl;
	cout << "\t| ���� ���� �����Դϴ�. |" << endl;
	cout << "=============================================" << endl;
	cout << "�������� 5�� �̻��� �Ǹ� ���ӿ��� �¸��մϴ�." << endl;
	cout << "=============================================" << endl;
	cout << "1. EASY \t- ������ ���� ����" << endl;
	cout << "2. NORMAL \t- ���� ���θ� ���� ���� ����" << endl;
	cout << "3. HARD \t- �밢���� ���� ���� ����" << endl;
	cout << "4. HELL \t- 0.5�� ���� ���� ���� �� �� ����" << endl;
	cout << "5. XLINEMOVE \t- X �� �ϳ��� ���������� �̵�" << endl;
	cout << "6. YLINEMOVE \t- Y �� �ϳ��� ���������� �̵�" << endl;
	cout << "AI ��带 �����ϼ��� : ";

	int mode = 0;
	cin >> mode;

	if (mode == EASY) {
		baseMode(EASY);
	}
	else if (mode == NORMAL) {
		baseMode(NORMAL);
	}
	else if (mode == HARD) {
		baseMode(HARD);
	}
	else if (mode == HELL) {
		baseMode(HELL);
	}
	else if (mode == XLINEMOVE) {
		baseMode(XLINEMOVE);
	}
	else if (mode == YLINEMOVE) {
		baseMode(YLINEMOVE);
	}
	else {
		cout << "�߸� �� ���� �Է��ϼ̽��ϴ�. " << endl;
		Sleep(1000);
		startScreen();
	}
}

/// <summary>
/// ���� ���� �� ���ڰ� �̹� ���õǾ��� �������� Ȯ��
/// </summary>
/// <param name="selectNumber"></param>
/// <returns></returns>
bool checkNumber(int selectNumber) {

	// 0�̸� ����
	if (selectNumber == 0) {
		cout << "������ ����˴ϴ�." << endl;
		exit(0);
	}

	// 1���� �۰ų� 25���� ũ�� false
	if (selectNumber < 1 || 25 < selectNumber) {
		return false;
	}

	// possibleBingo �ȿ� ���� �ִٸ� true
	for (int i = 0; i < possibleIndex; i++) {
		if (selectNumber == possibleBingo[i])
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// ���õ��� ���� ���ڵ� ã��
/// </summary>
void findNumber() {
	possibleIndex = 0;

	// �ʱ�ȭ
	for (int i = 0; i < 25; i++) {
		possibleBingo[i] = 0;
	}

	// ���õ��� ���� �� ã��
	for (int i = 0; i < 25; i++) {
		if (playerBingo[i] != END_NUMBER) {
			possibleBingo[possibleIndex++] = playerBingo[i];
		}
	}

	// ����
	for (int i = 0; i < possibleIndex - 1;) {
		if (possibleBingo[i] <= possibleBingo[i + 1]) {
			i++;
			continue;
		}

		int temp = possibleBingo[i];
		possibleBingo[i] = possibleBingo[i + 1];
		possibleBingo[i + 1] = temp;
		i = 0;
	}
}

/// <summary>
/// �⺻ ��� - ������ ��� �� ���â
/// </summary>
void basePrint(int mode) {

	system("cls");
	cout << "============== Player ==============";
	printBingo(playerBingo, mode);
	int playerBingoCount = checkBingo(playerBingo);
	cout << "Bingo Line : " << playerBingoCount << endl << endl << endl;

	cout << "================ AI ================";
	printBingo(aiBingo, mode);
	int aiBingoCount = checkBingo(aiBingo);
	cout << "Bingo Line : " << aiBingoCount << endl << endl;

	if (playerBingoCount >= 5) {
		cout << "�÷��̾ �̰���ϴ�." << endl;
		exit(0);
	}
	else if (aiBingoCount >= 5) {
		cout << "AI�� �̰���ϴ�." << endl;
		exit(0);
	}
}

/// <summary>
/// ��� ��忡�� �����ϰ� �ϴ� ��
/// </summary>
void base(int mode) {

	system("cls");

	basePrint(mode);

	for (int i = 0; i < _count; i++) {
		cout << "�� " << countArr[i] << " ";
	}

	cout << endl << "���� �Էµ��� ���� �� : ";
	findNumber();

	for (int i = 0; i < possibleIndex; i++) {
		cout << possibleBingo[i] << " ";
	}

	cout << endl << "���ڸ� �Է��ϼ���(0 : ����) : ";

	int selectNumber = 0;
	cin >> selectNumber;

	if (!checkNumber(selectNumber)) {
		cout << "�ٸ� ���� �Է��ϼ���." << endl;
		Sleep(1000);
		base(mode);
	}

	countArr[_count++] = selectNumber;

	changeBingo(playerBingo, selectNumber);
	changeBingo(aiBingo, selectNumber);

	basePrint(mode);

	Sleep(1000);
}

/// <summary>
/// AI�� ������ ���� �̹� *���� Ȯ��
/// </summary>
/// <param name="aiSelectNumber"></param>
/// <returns></returns>
bool checkAiSelectNumber(int aiSelectNumber) {
	for (int i = 0; i < possibleIndex; i++) {
		if (aiSelectNumber == possibleBingo[i]) {

			countArr[_count++] = aiSelectNumber;

			for (int i = 0; i < _count; i++) {
				cout << "�� " << countArr[i] << " ";
			}

			cout << endl << "AI�� ������ �� : " << aiSelectNumber;
			Sleep(1000);
			changeBingo(playerBingo, aiSelectNumber);
			changeBingo(aiBingo, aiSelectNumber);
			return true;
		}
	}
	return false;
}

/// <summary>
/// ��忡 ���� �޶����� AI ���� ����
/// </summary>
/// <param name="mode"></param>
/// <returns></returns>
int aiSelectNumber(int mode) {
	int selectNumber = 0;

	int max = 0;

	if (mode == EASY) {
		cout << "���� �߻�" << endl;
		exit(0);
	}

	// NORMAL�� ������ �ڵ�
	for (int i = 0; i < 5; i++) {
		if (max <= bingoXLine[i][0] && bingoXLine[i][0] < 5) {
			max = bingoXLine[i][0];
			selectNumber = bingoXLine[i][1];
		}
		if (max <= bingoYLine[i][0] && bingoYLine[i][0] < 5) {
			max = bingoYLine[i][0];
			selectNumber = bingoYLine[i][1];
		}
	}

	if (mode == HARD || mode == HELL || mode == XLINEMOVE || mode == YLINEMOVE) {
		for (int i = 0; i < 2; i++) {
			if (max <= bingoDLine[i][0] && bingoDLine[i][0] < 5) {
				max = bingoDLine[i][0];
				selectNumber = bingoDLine[i][1];
			}
		}
	}



	return selectNumber;
}

/// <summary>
/// AI�� ��忡 ���� ���� ����
/// </summary>
/// <param name="mode"></param>
void aiSelectNumberBase(int mode) {

	findNumber();

	int selectNumber = 0;

	// AI�������� ���Ͱ� �Ǿ� �ִٸ�
	if (!checkAiSelectNumber(aiBingoCenter)) {
		if (mode == EASY) {
			// EASY ����� �� ������ ���� ���
			selectNumber = rand() % 25 + 1;
		}
		else if (mode == NORMAL) {
			// NORMAL ����� �� ���� ���θ� ���� ���� ���� ���
			selectNumber = aiSelectNumber(NORMAL);
		}
		else if (mode == HARD) {
			// HARD ����� �� �밢���� �����Ͽ� ���� ���� ���� ���
			selectNumber = aiSelectNumber(HARD);
		}
		else if (mode == HELL) {
			// HELL ����� �� �÷��̾��� ���� ���Ͽ� ���� ���
			selectNumber = aiSelectNumber(HELL);
		}
		else if (mode == XLINEMOVE) {
			selectNumber = aiSelectNumber(XLINEMOVE);
		}
		else if (mode == YLINEMOVE) {
			selectNumber = aiSelectNumber(YLINEMOVE);
		}

		if (!checkAiSelectNumber(selectNumber)) {
			aiSelectNumberBase(mode);
		}
	}
}

/// <summary>
/// ��� 
/// </summary>
/// <param name="mode"></param>
void baseMode(int mode) {

	Sleep(500);
	basePrint(NULL);

	if (mode == HELL) {
		cout << endl << "���ڰ� 0.5�� �� #���� ��ȯ�˴ϴ�." << endl;
		Sleep(500);
	}

	base(mode);

	aiSelectNumberBase(mode);
	Sleep(1000);

	if (mode == XLINEMOVE) {
		bingoXLineMove(playerBingo);
		_countX++;
	}
	else if (mode == YLINEMOVE) {
		bingoYLineMove(playerBingo);
		_countY++;
	}

	baseMode(mode);
}

int main() {
	srand((unsigned int)time(NULL));

	setBingo(playerBingo);
	setBingo(aiBingo);

	startScreen();

	return 0;
}