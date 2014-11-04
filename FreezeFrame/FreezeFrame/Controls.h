#pragma once

struct Controls{
	UCHAR up,down,left,right,walk;
	Controls(UCHAR U, UCHAR D, UCHAR L, UCHAR R,UCHAR W){
		up = U;
		down = D;
		left = L;
		right = R;
		walk = W;
	}
	Controls(){};
};