#ifndef Utils_h
#define Utils_h

namespace GraphUtils
{
	// ��������� ���� ������, �� ���������� �������:
	extern float diffWhite[];
	extern float ambiWhite[];
	extern float specWhite[];

	extern float diffBlack[];
	extern float ambiBlack[];
	extern float specBlack[];

	extern float diffBlue[];
	extern float ambiBlue[];
	extern float specBlue[];

	extern float diffGray[];
	extern float ambiGray[];
	extern float specGray[];

	extern float diffRed[];
	extern float ambiRed[];
	extern float specRed[];

	extern float diffYellow[];
	extern float ambiYellow[];
	extern float specYellow[];

	extern float diffGreen[];
	extern float ambiGreen[];
	extern float specGreen[];

	extern float diffOrange[];
	extern float ambiOrange[];
	extern float specOrange[];

	extern float diffLightBlue[];
	extern float ambiLightBlue[];
	extern float specLightBlue[];

	extern float diffViolet[];
	extern float ambiViolet[];
	extern float specViolet[];

	const float shininess = 70; // �����

	// ��������� "���������" ������������ ������ ����� �����
	void shuffle(int *a, int size);

	// ��������� �������������
	void parallelepiped(float length, float width, float height);

	// ³���������� ����� ������ �������� ������� � ������� �������
	void drawString(void *font, const char* text, float x, float y);
}

#endif
