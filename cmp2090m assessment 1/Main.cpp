#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

using namespace std;

#include "Matrix.h"

//e.g. for task 1: logo_with_noise and logo_shuffled, M = 512, N = 512
//e.g. for task 2, Cluttered_scene, M = 768, N = 1024
//e.g. for task 2, Wally_grey, M = 49, N =  36

void DisplayMainMenu()
{
	cout << "MAIN MENU!\n\n";
	cout << "1. Jigsaw Solving\n";
	cout << "2. Template Matching\n\n";
	cout << "0. Exit\n\n";
	cout << "Pick one: ";
}

void JigSawSolving()
{
	cout << "Converting txt to matrixes. Please wait..." << endl;
	Matrix logoShuffled(512, 512, "logo_shuffled.txt");
	Matrix logoWithNoise(512, 512, "logo_with_noise.txt", 150, 255, 0);
	int closestMatchLocM, closestMatchLocN;  
	double currentMatch, closestMatch;

	logoWithNoise.WritePGM("logoWithLessNoise.pgm", 255);
	logoShuffled.WritePGM("logoShuffled.pgm", 255);

	cout << "Beginning unscramble. Please wait..." << endl;
	for (int m = 0; m < logoWithNoise.getSizeR(); m += 32)
		for (int n = 0; n < logoWithNoise.getSizeC(); n += 32)
		{
			closestMatch = NULL;
			Matrix noiseSegment(logoWithNoise, n, m, n + 31, m + 31);

			for (int m2 = 0; m2 < logoShuffled.getSizeR(); m2 += 32)
				for (int n2 = 0; n2 < logoShuffled.getSizeC(); n2 += 32)
				{
					Matrix shuffledSegment(logoShuffled, n2, m2, n2 + 31, m2 + 31);
					Matrix noiseySegmentTemp = noiseSegment;
					double sumTop = 0, sumBottom1 = 0, sumBottom2 = 0;

					Matrix noiseySegmentTempSubMean = noiseySegmentTemp - noiseySegmentTemp.findMean();
					Matrix shuffledSegmentSubMean = shuffledSegment - shuffledSegment.findMean();

					for (int x = 0; x < (shuffledSegment.getSizeC() * shuffledSegment.getSizeR()); x++)
					{
						sumTop += (noiseySegmentTemp.getPixel(x)*shuffledSegment.getPixel(x));
						sumBottom1 += (noiseySegmentTemp.getPixel(x)*noiseySegmentTempSubMean.getPixel(x));
						sumBottom2 += (shuffledSegment.getPixel(x)*shuffledSegmentSubMean.getPixel(x));
					}
					double NC = sumTop / sqrt(sumBottom1*sumBottom2);

					if ((NC > closestMatch) || (closestMatch == NULL))
					{
						//cout << currentMatch << " " << closestMatch << endl;
						closestMatch = NC;
						closestMatchLocM = m2;
						closestMatchLocN = n2;
					}

					
				}
			logoShuffled.switchSegment(m, n, closestMatchLocM, closestMatchLocN, 32, 32);

			/*cout << m << n << endl;*/
		}

	logoShuffled.WritePGM("unshuffledLogo.pgm", 255);
}

void TemplateMatching()
{
	double currentSSD;
	double closestMatch = NULL;
	int closestMatchM, closestMatchN;

	cout << "Converting txt files to Matrix. Please wait..." << endl;
	Matrix clutteredScene(768, 1024, "Cluttered_scene.txt");
	Matrix wally(49, 36, "Wally_grey.txt");
	Matrix wallyBin(49, 36, "Wally_grey.txt", 255, 0, 1);

	cout << "Beginning Search Now!" << endl;
	for (int m = 0; m <(clutteredScene.getSizeR() - wally.getSizeR()); m++)//loop through all rows within startM endM range
	{
		for (int n = 0; n < (clutteredScene.getSizeC() - wally.getSizeC()); n++)
		{
			Matrix clutteredSceneSegment(clutteredScene, n, m, (n + wally.getSizeC() - 1), (m + wally.getSizeR() - 1));
			clutteredSceneSegment = clutteredSceneSegment * wallyBin;

			Matrix SSD = clutteredSceneSegment - wally;
			currentSSD = SSD.CalculateVariance();
			if (currentSSD < closestMatch || closestMatch == NULL)
			{
				if (closestMatch != NULL)
					cout << "Potential Wally identified. Lower the better: "<< closestMatch << endl;
				closestMatch = currentSSD;
				closestMatchM = m;
				closestMatchN = n;
				clutteredSceneSegment.WritePGM("thing.pgm", 255);
			}
		}
	}

	clutteredScene.whiteOutSegment(wally, closestMatchM, closestMatchN);

	clutteredScene.WritePGM("wallyfound.pgm", 255);
}



void main(){
	cout.precision(6);
	int mainMenuChoice = 1;

	while (mainMenuChoice != 0)
	{
		DisplayMainMenu();
		cin >> mainMenuChoice;


		switch (mainMenuChoice)
		{
		case 1:
			cout << "you chose Jigsaw Solving. Please wait..." << endl;
			JigSawSolving();
			break;
		case 2:
			cout << "you chose Template Matching. Please wait..." << endl;
			TemplateMatching();
			break;
		case 0:
			cout << "You chose to exit." << endl;
			break;
		default:
			cout << "you chose incorrectly" << endl;
			break;
		}
	}
	
	system("pause");
}
//Matrix shuffledSegment(logoShuffled, n2, m2, n2 + 31, m2 + 31);
//
//Matrix varience = shuffledSegment - noiseSegment;
//currentMatch = varience.CalculateVariance();
//if ((currentMatch <= closestMatch) || (closestMatch == NULL))
//{
//	//cout << currentMatch << " " << closestMatch << endl;
//	closestMatch = currentMatch;
//	closestMatchLocM = m2;
//	closestMatchLocN = n2;
//}