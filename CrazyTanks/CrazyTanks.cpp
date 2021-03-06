// CrazyTanks.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "CrazyTanks.h"

using namespace std;

int main()
{
	srand(time(0));
	mainMenu();
	fieldDefault(mainfieldArr);
	randWallPosition(mainfieldArr);
	CheckAIPanzerPosition();
	transfusion();

	while (!gameOver)
	{
		Sleep(_sleepScale);
		iTimer += 0.1;	
		system("cls");
		cout << endl << "\t   == CrazyTanks! ==\n   == For Luxoft, by LuckyChack. == " << endl;

		fieldPrint(reservfieldArr);		//вывод всей массы на экран
		if (!bReady)
		{
			cout << "Ready? <y>" << endl; char cReady;
			cin >> cReady;
			if (cReady == 'y')
				bReady = true;
		}

		cout << "HealthPoints: " << iPlayerHit << endl << endl;
		cout << "Enemies left: " << 6 - iEnemyHit << endl << "Kills: " << iEnemyHit << endl << endl;
			
		cout << "Time: " << setw(2) << iTimer  << " sec" << endl;

		PlayersPanzer(reservfieldArr, pPanzerX, pPanzerY);		    // проверка на изменения координат танка ГГ
 		shooting();

		AIPanzer(reservfieldArr, firstAIPanzerX, firstAIPanzerY);	//вражеские такни от 1...6 штук.
		AIPanzer(reservfieldArr, secondAIPanzerX, secondAIPanzerY);
		AIPanzer(reservfieldArr, thirdAIPanzerX, thirdAIPanzerY);
		AIPanzer(reservfieldArr, forthAIPanzerX, forthAIPanzerY);
		AIPanzer(reservfieldArr, fithAIPanzerX, fithAIPanzerY);
		AIPanzer(reservfieldArr, sixthAIPanzerX, sixthAIPanzerY);

		transfusion();
	}
	GameOver();
	/*system("pause");*/
	return 0;
}


/////////////////Работа с основными масивами///////////////////////
//																 //

int mainMenu()
{
	char cAnswer;
	cout << endl << "\t== CrazyTanks! v.1.0 ==\n    == For Luxoft, by LuckyChack. == " << endl << endl;
	cout << "Controlers: 'Up, Down, Left, Right' buttons - to move your tank. 'SPACE' - to shoot." << endl;
	cout << "  Unfortunately, your tank is too old and can shoot only up (the same as enemy tanks)." << endl;
	cout << "  Your target is to kill all enemies. And be cereful, you have just 3 Health Points." << endl << endl;
	cout << "We will not make you wait a long time for update to v.2.0. ;)" << endl << endl;
	cout << "Continue?  <y,n>" << endl;
	cin >> cAnswer;
	if (cAnswer == 'y')
		return 0;
	else if (cAnswer == 'n')
	{
		gameOver = true;
		return 0;
	}
}

void fieldDefault(char(*fieldDefArr)[fieldY])
{
	pPanzerX = fieldX - 2;
	pPanzerY = fieldY / 2;

	for (int x = 0; x < fieldX; x++)
		for (int y = 0; y < fieldY; y++)
		{
			if (x == 0 || x == fieldX - 1 || y == 0 || y == fieldY - 1)
				fieldDefArr[x][y] = wallSymb;
			else
				fieldDefArr[x][y] = ' ';
		}
}

int randWallLocationX()
{
	positionX = rand() % fieldX - 1;
	return positionX;
}

int randWallLocationY()
{
	positionY = rand() % fieldY - 1;
	return positionY;
}

void randWallPosition(char(*locationArr)[fieldY])
{
	for (int i = 1; i < MaxWallSize; i++)
		CheckWallPosition(locationArr, randWallLocationX(), randWallLocationY(), 4);
	for (int i = -1; i < MaxWallSize; i++)
		CheckWallPosition(locationArr, randWallLocationX(), randWallLocationY(), 3);
	for (int i = -1; i < MaxWallSize; i++)
		CheckWallPosition(locationArr, randWallLocationX(), randWallLocationY(), 2);
	for (int i = 0; i < MaxWallSize; i++)
		CheckWallPosition(locationArr, randWallLocationX(), randWallLocationY(), 1);
}

void CheckWallPosition(char(*CheckWallPosArr)[fieldY], int WallLocationX, int WallLocationY, int WallSize)
{
	bool bLocationCheck = false;
	bool horLocationCheck = false;
	bool verLocationCheck = false;
	bool orientation = rand() % 2;

	while (!bLocationCheck)
	{
		for (int x = 0; x < WallSize; x++)
			if (   CheckWallPosArr[orientation ? WallLocationX - 1 : WallLocationY + x][orientation ? WallLocationY + x : WallLocationX + 1] != wallSymb
				&& CheckWallPosArr[orientation ? WallLocationX : WallLocationY + x]	   [orientation ? WallLocationY + x : WallLocationX] != wallSymb
				&& CheckWallPosArr[orientation ? WallLocationX + 1 : WallLocationY + x][orientation ? WallLocationY + x : WallLocationX + 1] != wallSymb)
				horLocationCheck = true;
			else
			{
				horLocationCheck = false;
				break;
			}

		for (int x = 0; x < WallSize; x++)
			if (   CheckWallPosArr[orientation ? (WallLocationX - 1) + x : WallLocationY + 1][orientation ? WallLocationY + 1 : (WallLocationX - 1) + x] != wallSymb
				&& CheckWallPosArr[orientation ? (WallLocationX - 1) + x : WallLocationY + WallSize - 1][orientation ? WallLocationY + WallSize - 1 : (WallLocationX - 1) + x] != wallSymb)
				verLocationCheck = true;
			else
			{
				verLocationCheck = false;
				break;
			}


		if (verLocationCheck && horLocationCheck)
		{
			for (int x = 0; x < WallSize; x++)
				CheckWallPosArr[orientation ? WallLocationX : WallLocationY + x][orientation ? WallLocationY + x : WallLocationX] = wallSymb;
			bLocationCheck = true;
		}
		else
		{
			WallLocationX = randWallLocationX();
			WallLocationY = randWallLocationY();
		}
	}
}

void transfusion()
{
	for (int x = 0; x < fieldX; x++)
		for (int y = 0; y < fieldY; y++)
			reservfieldArr[x][y] = mainfieldArr[x][y];
}

void fieldPrint(char(*fieldPrintArr)[fieldY])
{
	for (int x = 0; x < fieldX; x++)
	{
		for (int y = 0; y < fieldY; y++)
		{
			if (x == pPanzerX && y == pPanzerY)
				fieldPrintArr[x][y] = PlayersPanzerSymb;		//принт танка ГГ
			if(!bEnemyHit1)
				if (x == firstAIPanzerX && y == firstAIPanzerY)		//принт вражеских 6ти танков
					fieldPrintArr[x][y] = enemyPanzerSymb;
			if(!bEnemyHit2)
				if (x == secondAIPanzerX && y == secondAIPanzerY)
					fieldPrintArr[x][y] = enemyPanzerSymb;
			if(!bEnemyHit3)
				if (x == thirdAIPanzerX && y == thirdAIPanzerY)
					fieldPrintArr[x][y] = enemyPanzerSymb;
			if(!bEnemyHit4)
				if (x == forthAIPanzerX && y == forthAIPanzerY)
					fieldPrintArr[x][y] = enemyPanzerSymb;
			if(!bEnemyHit5)
				if (x == fithAIPanzerX && y == fithAIPanzerY)
					fieldPrintArr[x][y] = enemyPanzerSymb;
			if(!bEnemyHit6)
				if (x == sixthAIPanzerX && y == sixthAIPanzerY)
					fieldPrintArr[x][y] = enemyPanzerSymb;

			for (int z = 0; z < ammoValue; z++)					//принт пуль
			{
				if (x == AmmoSymbX[z] && y == AmmoSymbY[z])
				{
					fieldPrintArr[x][y] = AmoSymb;
				}
			}
			for (int q = 0; q < AIammoValue; q++)					//принт пуль
			{
				if (x == AIAmmoSymbX[q] && y == AIAmmoSymbY[q])
				{
					fieldPrintArr[x][y] = AmoSymb;
				}
			}

			cout << fieldPrintArr[x][y] << " ";
		}
		cout << endl;
	}
}
//																 //
///////////////////////////////////////////////////////////////////



//////////////////////////Танк ГГ/////////////////////////////////
//																//

void shooting()
{
		for (int z = 0; z < ammoValue; z++)					//принт пуль
		{
  			/*if (bWasUP && bFireCheck)
			{*/
				pFireX = AmmoSymbX[z];
				prevpFireX[z] = pFireX;
				pFireX--;
				AmmoSymbX[z] = pFireX;
				bFireCheck = false, bWasUP = false; bWasDown = false, bWasRight = false, bWasLeft = false;
				
				pFireY = AmmoSymbY[z];
			/*}
			else if (prevpFireX[z] > pFireX && pFireX != 0)
			{
				prevpFireX[z] = pFireX;
				pFireX--;
				AmmoSymbX[z] = pFireX;
			}
				
			if (bWasDown)
			{
				if (bFireCheck)
				{
					pFireX = AmmoSymbX[z];
					prevpFireX[z] = pFireX;
					pFireX++;
					AmmoSymbX[z] = pFireX;
					bFireCheck = false, bWasUP = false; bWasDown = false, bWasRight = false, bWasLeft = false;
				}
			}
			else if (prevpFireX[z] < pFireX && pFireX != 0)
			{
				//prevpFireX[z] = pFireX;
				pFireX++;
				AmmoSymbX[z] = pFireX;
			}

			if (bWasRight)
			{
				if (bFireCheck)
				{
					pFireY = AmmoSymbY[z];
					prevpFireY[z] = pFireY;
					pFireY++;
					AmmoSymbY[z] = pFireY;
					bFireCheck = false, bWasUP = false; bWasDown = false, bWasRight = false, bWasLeft = false;
				}
			}
			else if (prevpFireY[z] < pFireY && pFireY != 0)
			{
				//prevpFireY[z] = pFireY;
				pFireY++;
				AmmoSymbY[z] = pFireY;
			}

			if (bWasLeft)
			{
				if (bFireCheck)
				{
					pFireY = AmmoSymbY[z];
					prevpFireY[z] = pFireY;
					pFireY--;
					AmmoSymbY[z] = pFireY;
					bFireCheck = false, bWasUP = false; bWasDown = false, bWasRight = false, bWasLeft = false;
				}
			}
			else if (prevpFireY[z] > pFireY && pFireY != 0)
			{
				prevpFireY[z] = pFireY;
				pFireY--;
				AmmoSymbY[z] = pFireY;
			}*/

				if (mainfieldArr[pFireX][pFireY] == wallSymb)
				{
					AmmoSymbX[z] = 0;
					ammoValue--;
				}
				if (pFireX == firstAIPanzerX && pFireY == firstAIPanzerY )
				{
					AmmoSymbX[z] = 0;
					ammoValue--;
					iEnemyHit++;
					firstAIPanzerX = 0, firstAIPanzerY = 0;
					bEnemyHit1 = true;
				}
				if (pFireX == secondAIPanzerX && pFireY == secondAIPanzerY)
				{
					AmmoSymbX[z] = 0;
					ammoValue--;
					iEnemyHit++;
					secondAIPanzerX = 0, secondAIPanzerY = 0;
					bEnemyHit2 = true;
				}
				if (pFireX == thirdAIPanzerX && pFireY == thirdAIPanzerY)
				{
					AmmoSymbX[z] = 0;
					ammoValue--;
					iEnemyHit++;
					thirdAIPanzerX = 0, thirdAIPanzerY = 0;
					bEnemyHit3 = true;
				}
				if (pFireX == forthAIPanzerX && pFireY == forthAIPanzerY)
				{
					AmmoSymbX[z] = 0;
					ammoValue--;
					iEnemyHit++;
					forthAIPanzerX = 0, forthAIPanzerY = 0;
					bEnemyHit4 = true;
				}
				if (pFireX == fithAIPanzerX && pFireY == fithAIPanzerY)
				{
					AmmoSymbX[z] = 0;
					ammoValue--;
					iEnemyHit++;
					fithAIPanzerX = 0, fithAIPanzerY = 0;
					bEnemyHit5 = true;
				}
				if (pFireX == sixthAIPanzerX && pFireY == sixthAIPanzerY)
				{
					AmmoSymbX[z] = 0;
					ammoValue--;
					iEnemyHit++;
					sixthAIPanzerX = 0, sixthAIPanzerY = 0;
					bEnemyHit6 = true;
				}
				if (iEnemyHit == 6)
				{
					winner = true;
					gameOver = true;
				}
		}
}

void PlayersPanzer(char(*playersPanzerArr)[fieldY], int playersPanzerLocationX, int playersPanzerLocationY)
{
	if (_kbhit())
		switch (_getch())
		{
		case UP:
			bWasUP = true;
			if (playersPanzerArr[pPanzerX - 1][pPanzerY] != wallSymb && playersPanzerArr[pPanzerX - 1][pPanzerY] != enemyPanzerSymb)
				pPanzerX--;
			break;

		case DOWN:
			bWasDown = true;
			if (playersPanzerArr[pPanzerX + 1][pPanzerY] != wallSymb && playersPanzerArr[pPanzerX + 1][pPanzerY] != enemyPanzerSymb)
				pPanzerX++;
			break;

		case RIGHT:
			bWasRight = true;
			if (playersPanzerArr[pPanzerX][pPanzerY + 1] != wallSymb && playersPanzerArr[pPanzerX][pPanzerY + 1] != enemyPanzerSymb)
				pPanzerY++;
			break;

		case LEFT:
			bWasLeft = true;
			if (playersPanzerArr[pPanzerX][pPanzerY - 1] != wallSymb && playersPanzerArr[pPanzerX][pPanzerY - 1] != enemyPanzerSymb)
				pPanzerY--;
			break;

		case FIRE:
			bFireCheck = true;

			prevAmmoSymbX = AmmoSymbX[0];
			prevAmmoSymbY = AmmoSymbY[0];

			AmmoSymbX[0] = pPanzerX;
			AmmoSymbY[0] = pPanzerY;
						
			for (int i = 1; i <= ammoValue; i++)
			{
				prev2AmmoSymbX = AmmoSymbX[i];
				prev2AmmoSymbY = AmmoSymbY[i];

				AmmoSymbX[i] = prevAmmoSymbX;
				AmmoSymbY[i] = prevAmmoSymbY;
				
				prevAmmoSymbX = prev2AmmoSymbX;
				prevAmmoSymbY = prev2AmmoSymbY;
			}
			ammoValue++;
			
			cout << "BANG!!!";
			break;
		}




	if (AmmoSymbX[i] == pPanzerX && AmmoSymbY[i] == pPanzerY)
	{
		//gameOver = true;
		//GameOver();
	}
}

//																 //
///////////////////////////////////////////////////////////////////



///////////////////Вражеские танки////////////////////////////////
//																//

int randAIPanzerLocationX()				 //рандомная координата Х для вражеских танков
{
	AIPositionX = rand() % (fieldY - 2) + 1;	
	return AIPositionX;
}	  

int randAIPanzerLocationY()				 //рандомная координата У для вражеских танков
{
	AIPositionY =  rand() % (fieldY - 2) + 1;
	return AIPositionY;
}

//проверка на возможность расположения вражеских танков в координатах Х, У.
void randAIPanzerPosition(char(*AIPanzerArr)[fieldY], int AIPanzerLocationX, int AIPanzerLocationY, int& setUpX, int& setUpY)		
{
	bool panzerCheck = false;
	bool panzerWallCheck = false;
	bool totalpanzerCheck = false;
	 
	while (!totalpanzerCheck)
	{
		for (int x = 0; x < panzerSize; x++)
		{
			if (   reservfieldArr[AIPanzerLocationX - 2][(AIPanzerLocationY - 2) + x] != enemyPanzerSymb
				&& reservfieldArr[AIPanzerLocationX - 1][(AIPanzerLocationY - 2) + x] != enemyPanzerSymb
				&& reservfieldArr[AIPanzerLocationX]    [(AIPanzerLocationY - 2) + x] != enemyPanzerSymb
				&& reservfieldArr[AIPanzerLocationX + 1][(AIPanzerLocationY - 2) + x] != enemyPanzerSymb
				&& reservfieldArr[AIPanzerLocationX + 2][(AIPanzerLocationY - 2) + x] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX][AIPanzerLocationY] != wallSymb)						
				panzerCheck = true;
			else
			{
				panzerCheck = false;
				break;
			}
		}
			
		if (panzerCheck)
		{
			setUpX = AIPanzerLocationX;
			setUpY = AIPanzerLocationY;
			reservfieldArr[setUpX][setUpY] = enemyPanzerSymb;
						
			totalpanzerCheck = true;
		}
		else
		{
			AIPanzerLocationX = randAIPanzerLocationX();
			AIPanzerLocationY = randAIPanzerLocationY();
		}
		panzerCheck = false;
		panzerWallCheck = false;
	}
}
			
void CheckAIPanzerPosition()
{
	randAIPanzerPosition(mainfieldArr, randAIPanzerLocationX(), randAIPanzerLocationY(), firstAIPanzerX, firstAIPanzerY);
	randAIPanzerPosition(mainfieldArr, randAIPanzerLocationX(), randAIPanzerLocationY(), secondAIPanzerX, secondAIPanzerY);
	randAIPanzerPosition(mainfieldArr, randAIPanzerLocationX(), randAIPanzerLocationY(), thirdAIPanzerX, thirdAIPanzerY);
	randAIPanzerPosition(mainfieldArr, randAIPanzerLocationX(), randAIPanzerLocationY(), forthAIPanzerX, forthAIPanzerY);
	randAIPanzerPosition(mainfieldArr, randAIPanzerLocationX(), randAIPanzerLocationY(), fithAIPanzerX, fithAIPanzerY);
	randAIPanzerPosition(mainfieldArr, randAIPanzerLocationX(), randAIPanzerLocationY(), sixthAIPanzerX, sixthAIPanzerY);
}

void AIPanzer(char AIPanzerArr [fieldX][fieldY], int& AIPanzerLocationX, int& AIPanzerLocationY)
{
	randWalkOnMap = rand() % 4 + 1;

	for(; i <= 2; i++)
	{
		switch (randWalkOnMap)
		{
		case 1:
			if (   AIPanzerArr[AIPanzerLocationX - i][AIPanzerLocationY - 2] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - i][AIPanzerLocationY - 1] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - i][AIPanzerLocationY]	 != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - i][AIPanzerLocationY + 1] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - i][AIPanzerLocationY + 2] != enemyPanzerSymb

				&& AIPanzerArr[AIPanzerLocationX - i][AIPanzerLocationY - 2] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - i][AIPanzerLocationY - 1] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - i][AIPanzerLocationY]	 != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - i][AIPanzerLocationY - 1] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - i][AIPanzerLocationY - 2] != PlayersPanzerSymb

				&& AIPanzerArr[AIPanzerLocationX - 1][AIPanzerLocationY]     != wallSymb)
				caseUPCheck = true;
			else
			{
				caseUPCheck = false;
				i = 3;
				break;
			}
			break;
		case 2:
			if (   AIPanzerArr[AIPanzerLocationX + i][AIPanzerLocationY - 2] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + i][AIPanzerLocationY - 1] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + i][AIPanzerLocationY]	 != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + i][AIPanzerLocationY + 1] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + i][AIPanzerLocationY + 2] != enemyPanzerSymb
				
				&& AIPanzerArr[AIPanzerLocationX + i][AIPanzerLocationY - 2] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + i][AIPanzerLocationY - 1] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + i][AIPanzerLocationY] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + i][AIPanzerLocationY - 1] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + i][AIPanzerLocationY - 2] != PlayersPanzerSymb
				
				&& AIPanzerArr[AIPanzerLocationX + 1][AIPanzerLocationY] != wallSymb)
				caseDownCheck = true;
			else
			{
				caseDownCheck = false;
				i = 3;
				break;
			}
			break;
		case 3:
			if (   AIPanzerArr[AIPanzerLocationX - 2][AIPanzerLocationY + i] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - 1][AIPanzerLocationY + i] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX]    [AIPanzerLocationY + i] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + 1][AIPanzerLocationY + i] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + 2][AIPanzerLocationY + i] != enemyPanzerSymb

				&& AIPanzerArr[AIPanzerLocationX - 2][AIPanzerLocationY + i] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - 1][AIPanzerLocationY + i] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX]	 [AIPanzerLocationY + i] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + 1][AIPanzerLocationY + i] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + 2][AIPanzerLocationY + i] != PlayersPanzerSymb

				&& AIPanzerArr[AIPanzerLocationX][AIPanzerLocationY + 1] != wallSymb)
				caseRightCheck = true;
			else
			{
				caseRightCheck = false;
				i = 3;
				break;
			}
			break;
		case 4:
			if (   AIPanzerArr[AIPanzerLocationX - 2][AIPanzerLocationY - i] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - 1][AIPanzerLocationY - i] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX]    [AIPanzerLocationY - i] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + 1][AIPanzerLocationY - i] != enemyPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + 2][AIPanzerLocationY - i] != enemyPanzerSymb

				&& AIPanzerArr[AIPanzerLocationX - 2][AIPanzerLocationY - i] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX - 1][AIPanzerLocationY - i] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX]    [AIPanzerLocationY - i] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + 1][AIPanzerLocationY - i] != PlayersPanzerSymb
				&& AIPanzerArr[AIPanzerLocationX + 2][AIPanzerLocationY - i] != PlayersPanzerSymb

				&& AIPanzerArr[AIPanzerLocationX][AIPanzerLocationY - 1] != wallSymb)
				caseLeftCheck = true;
			else
			{
				caseLeftCheck = false;
				i = 3;
				break;
			}
			break;
		}
	}

		if(caseUPCheck)
			AIPanzerLocationX--;
		else if(caseDownCheck)
			AIPanzerLocationX++;
		else if (caseRightCheck)	
			AIPanzerLocationY++;
		else if (caseLeftCheck)
			AIPanzerLocationY--;

		AIShoot(AIPanzerLocationX, AIPanzerLocationY);

	 caseUPCheck = false;
	 caseDownCheck = false;
	 caseRightCheck = false;
	 caseLeftCheck = false;
	 i = 1;
}

void AIShoot(int AIpanzerLocationX, int AIpanzerLocationY)
{
	if (AIpanzerLocationY == pPanzerY && AIpanzerLocationX > pPanzerX )
	{
		AIprevAmmoSymbX = AIAmmoSymbX[0];
		AIprevAmmoSymbY = AIAmmoSymbY[0];

		AIAmmoSymbX[0] = AIpanzerLocationX;
		AIAmmoSymbY[0] = AIpanzerLocationY;

		for (int i = 1; i <= AIammoValue; i++)
		{
			AIprev2AmmoSymbX = AIAmmoSymbX[i];
			AIprev2AmmoSymbY = AIAmmoSymbY[i];

			AIAmmoSymbX[i] = AIprevAmmoSymbX;
			AIAmmoSymbY[i] = AIprevAmmoSymbY;

			AIprevAmmoSymbX = AIprev2AmmoSymbX;
			AIprevAmmoSymbY = AIprev2AmmoSymbY;

		}
		AIammoValue++;

		//реализация выстрелов вверх
		for (int z = 0; z < AIammoValue; z++)					//принт пуль
		{
			pAIFireX = AIAmmoSymbX[z];
			AIprevpFireX[z] = pAIFireX;
			pAIFireX--;
			AIAmmoSymbX[z] = pAIFireX;

			pAIFireY = AIAmmoSymbY[z];

			if (reservfieldArr[pAIFireX][pAIFireY] == wallSymb)
			{
				AIAmmoSymbX[z] = 0;

				AIammoValue--;
			}
			if (pAIFireX == pPanzerX && pAIFireY == pPanzerY)
			{
				AIAmmoSymbX[z] = 0;
				AIammoValue--;
				iPlayerHit--;
			}
			if (iPlayerHit == 0)
			{
				gameOver = true;
				loser = true;
			}
		}
	}
	else
	{
		for (int z = 0; z < AIammoValue; z++)					//принт пуль
		{
			pAIFireX = AIAmmoSymbX[z];
			AIprevpFireX[z] = pAIFireX;
			pAIFireX--;
			AIAmmoSymbX[z] = pAIFireX;

			pAIFireY = AIAmmoSymbY[z];

			if (reservfieldArr[pAIFireX][pAIFireY] == wallSymb)
			{
				AIAmmoSymbX[z] = 0;

				AIammoValue--;
			}
			if (pAIFireX == pPanzerX && pAIFireY == pPanzerY)
			{
				AIAmmoSymbX[z] = 0;
				AIammoValue--;
				iPlayerHit--;
			}
			if (iPlayerHit == 0)
			{
				gameOver = true;
				loser = true;
			}
		}
	}
}
	
//																 //
///////////////////////////////////////////////////////////////////



void GameOver()
{
	if (winner)
	{
		system("cls");
		cout << "\n\n\tCongratulations!!! You win!!!\n\tYou kill all panzers!" << endl;
	}
	else if (loser)
	{
		system("cls");
		cout << "\n\n\tUnfortunately, you lose..." << endl;
	}
}