#pragma once

struct Controls{
	UCHAR up,down,left,right;
	Controls(UCHAR U, UCHAR D, UCHAR L, UCHAR R){
		up = U;
		down = D;
		left = L;
		right = R;
	}
	Controls(){};
};