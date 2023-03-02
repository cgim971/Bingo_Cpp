#include<iostream>
#include<time.h>
#include<Windows.h>

using namespace std;

// *이 될 값 :  값이 END_NUMBER라면 * 출력
const int END_NUMBER = 0;
// 플레이어 빙고판
int playerBingo[25] = { 0, };
// AI 빙고판
int aiBingo[25] = { 0, };

// 현재 선택 가능한 수들의 배열
int possibleBingo[25] = { 0, };
// 현재 선택 가능한 수들의 갯수
int possibleIndex = 0;

//빙고인지 확인
// 0번째 갯수 1번째 END_NUMBER가 아닌 수 2번째 플레이어의 갯수
int bingoXLine[5][3] = { 0, };
int bingoYLine[5][3] = { 0, };
int bingoDLine[2][3] = { 0, };

// AI 빙고판의 센터
int aiBingoCenter = 0;

// X축과 Y축 이동을 위해
int _countX = 0;
int _countY = 0;

// 출력된 숫자들의 배열
int countArr[25] = { 0, };
int _count = 0;

// 모드 구분을 위한 enum
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
/// 빙고판을 만듬
/// </summary>
/// <param name="arr"></param>
void setBingo(int arr[25]) {

	// 배열의 초기값 생성
	for (int i = 0; i < 25; i++) {
		arr[i] = i + 1;
	}

	// 스왑을 얼마나 할지 정함
	int count = rand() % 100 + 1;

	// 스왑
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
/// 빙고판 출력
/// </summary>
/// <param name="arr"></param>
void printBingo(int arr[25], int mode) {

	for (int i = 0; i < 25; i++) {

		if (i % 5 == 0) {
			cout << endl << endl;
		}

		if (arr[i] == END_NUMBER) {
			cout << "●";
		}
		else if (arr[i] < 10 && mode != HELL) {
			cout << 0 << arr[i];
		}
		else if (mode != HELL) {
			cout << arr[i];
		}
		else {
			cout << "□";
		}

		cout << "\t";
	}
	cout << endl << endl;
}

/// <summary>
/// X축 오른쪽으로 이동
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
/// Y축 아래로 이동
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
/// 빙고판의 숫자를 END_NUMBER로 변환
/// </summary>
/// <param name="arr"></param>
void changeBingo(int arr[25], int selectNumber) {

	// 변환
	for (int i = 0; i < 25; i++) {
		if (arr[i] == selectNumber) {
			arr[i] = END_NUMBER;
			break;
		}
	}
}

/// <summary>
/// 빙고 확인
/// </summary>
/// <param name="arr"></param>
/// <returns></returns>
int checkBingo(int arr[25]) {

	// 빙고 갯수
	int bingo = 0;

	// 빙고 라인 초기화
	for (int i = 0; i < 5; i++) {
		// 가로 선 초기화
		bingoXLine[i][0] = 0;

		// 세로 선 초기화
		bingoYLine[i][0] = 0;
		if (i < 2)
		{
			// 대각선 초기화
			bingoDLine[i][0] = 0;
		}
	}

	// 가로 세로 빙고 확인
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

	// 대각선 빙고 확인
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

	// 빙고 수 세기
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
/// 시작화면 출력
/// </summary>
void startScreen() {
	system("cls");
	cout << "=============================================" << endl;
	cout << "\t| 숫자 빙고 게임입니다. |" << endl;
	cout << "=============================================" << endl;
	cout << "빙고줄이 5줄 이상이 되면 게임에서 승리합니다." << endl;
	cout << "=============================================" << endl;
	cout << "1. EASY \t- 랜덤한 숫자 선택" << endl;
	cout << "2. NORMAL \t- 가로 세로만 따져 숫자 선택" << endl;
	cout << "3. HARD \t- 대각선도 따져 숫자 선택" << endl;
	cout << "4. HELL \t- 0.5초 동안 수가 보인 후 안 보임" << endl;
	cout << "5. XLINEMOVE \t- X 줄 하나씩 오른쪽으로 이동" << endl;
	cout << "6. YLINEMOVE \t- Y 줄 하나씩 오른쪽으로 이동" << endl;
	cout << "AI 모드를 선택하세요 : ";

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
		cout << "잘못 된 수를 입력하셨습니다. " << endl;
		Sleep(1000);
		startScreen();
	}
}

/// <summary>
/// 현재 선택 된 숫자가 이미 선택되었던 숫자인지 확인
/// </summary>
/// <param name="selectNumber"></param>
/// <returns></returns>
bool checkNumber(int selectNumber) {

	// 0이면 종료
	if (selectNumber == 0) {
		cout << "게임이 종료됩니다." << endl;
		exit(0);
	}

	// 1보다 작거나 25보다 크면 false
	if (selectNumber < 1 || 25 < selectNumber) {
		return false;
	}

	// possibleBingo 안에 값이 있다면 true
	for (int i = 0; i < possibleIndex; i++) {
		if (selectNumber == possibleBingo[i])
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// 선택되지 않은 숫자들 찾기
/// </summary>
void findNumber() {
	possibleIndex = 0;

	// 초기화
	for (int i = 0; i < 25; i++) {
		possibleBingo[i] = 0;
	}

	// 선택되지 않은 수 찾기
	for (int i = 0; i < 25; i++) {
		if (playerBingo[i] != END_NUMBER) {
			possibleBingo[possibleIndex++] = playerBingo[i];
		}
	}

	// 정렬
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
/// 기본 출력 - 빙고판 출력 및 결과창
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
		cout << "플레이어가 이겼습니다." << endl;
		exit(0);
	}
	else if (aiBingoCount >= 5) {
		cout << "AI가 이겼습니다." << endl;
		exit(0);
	}
}

/// <summary>
/// 모든 모드에서 동일하게 하는 일
/// </summary>
void base(int mode) {

	system("cls");

	basePrint(mode);

	for (int i = 0; i < _count; i++) {
		cout << "▶ " << countArr[i] << " ";
	}

	cout << endl << "현재 입력되지 않은 수 : ";
	findNumber();

	for (int i = 0; i < possibleIndex; i++) {
		cout << possibleBingo[i] << " ";
	}

	cout << endl << "숫자를 입력하세요(0 : 종료) : ";

	int selectNumber = 0;
	cin >> selectNumber;

	if (!checkNumber(selectNumber)) {
		cout << "다른 수를 입력하세요." << endl;
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
/// AI가 선택한 수가 이미 *인지 확인
/// </summary>
/// <param name="aiSelectNumber"></param>
/// <returns></returns>
bool checkAiSelectNumber(int aiSelectNumber) {
	for (int i = 0; i < possibleIndex; i++) {
		if (aiSelectNumber == possibleBingo[i]) {

			countArr[_count++] = aiSelectNumber;

			for (int i = 0; i < _count; i++) {
				cout << "▶ " << countArr[i] << " ";
			}

			cout << endl << "AI가 선택한 수 : " << aiSelectNumber;
			Sleep(1000);
			changeBingo(playerBingo, aiSelectNumber);
			changeBingo(aiBingo, aiSelectNumber);
			return true;
		}
	}
	return false;
}

/// <summary>
/// 모드에 따라 달라지는 AI 선택 조건
/// </summary>
/// <param name="mode"></param>
/// <returns></returns>
int aiSelectNumber(int mode) {
	int selectNumber = 0;

	int max = 0;

	if (mode == EASY) {
		cout << "오류 발생" << endl;
		exit(0);
	}

	// NORMAL과 동일한 코드
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
/// AI가 모드에 따라 수를 선택
/// </summary>
/// <param name="mode"></param>
void aiSelectNumberBase(int mode) {

	findNumber();

	int selectNumber = 0;

	// AI빙고판의 센터가 되어 있다면
	if (!checkAiSelectNumber(aiBingoCenter)) {
		if (mode == EASY) {
			// EASY 모드일 시 랜덤한 숫자 출력
			selectNumber = rand() % 25 + 1;
		}
		else if (mode == NORMAL) {
			// NORMAL 모드일 시 가로 세로만 조건 따져 숫자 출력
			selectNumber = aiSelectNumber(NORMAL);
		}
		else if (mode == HARD) {
			// HARD 모드일 시 대각선도 포함하여 조건 따져 숫자 출력
			selectNumber = aiSelectNumber(HARD);
		}
		else if (mode == HELL) {
			// HELL 모드일 시 플레이어의 빙고도 비교하여 숫자 출력
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
/// 모드 
/// </summary>
/// <param name="mode"></param>
void baseMode(int mode) {

	Sleep(500);
	basePrint(NULL);

	if (mode == HELL) {
		cout << endl << "숫자가 0.5초 뒤 #으로 변환됩니다." << endl;
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