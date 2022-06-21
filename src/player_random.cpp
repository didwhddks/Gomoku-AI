#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <array>
#include <vector>
using namespace std;

#define board array<array<int, 15>, 15>
#define ll long long

int player;
const int size = 15;

enum SPOT_STATE {
	EMPTY = 0,
	BLACK = 1,
	WHITE = 2
};

void dbg(const board& b) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j)
			cout << b[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}

bool check_bound(int i, int j) {
	return i >= 0 && i < size && j >= 0 && j < size;
}

bool check_win(const board& b) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (b[i][j] == BLACK) {
				// Down
				bool ok = 1;
				for (int k = 1; k <= 4 && ok; ++k) {
					if (i + k >= 15) ok = 0;
					else
						if (b[i + k][j] != BLACK) ok = 0;
				}
				if (ok) return true;
				ok = 1;
				// Right
				for (int k = 1; k <= 4 && ok; ++k) {
					if (j + k >= 15) ok = 0;
					else
						if (b[i][j + k] != BLACK) ok = 0;
				}
				if (ok) return true;
				ok = 1;
				// TopRight
				for (int k = 1; k <= 4 && ok; ++k) {
					if (i - k < 0 || j + k >= 15) ok = 0;
					else
						if (b[i - k][j + k] != BLACK) ok = 0;
				}
				if (ok) return true;
				ok = 1;
				// DownRight
				for (int k = 1; k <= 4 && ok; ++k) {
					if (i + k >= 15 || j + k >= 15) ok = 0;
					else
						if (b[i + k][j + k] != BLACK) ok = 0;
				}
				if (ok) return true;
			}
			else if (b[i][j] == WHITE) {
				// Down
				bool ok = 1;
				for (int k = 1; k <= 4 && ok; ++k) {
					if (i + k >= 15) ok = 0;
					else
						if (b[i + k][j] != WHITE) ok = 0;
				}
				if (ok) return true;
				ok = 1;
				// Right
				for (int k = 1; k <= 4 && ok; ++k) {
					if (j + k >= 15) ok = 0;
					else
						if (b[i][j + k] != WHITE) ok = 0;
				}
				if (ok) return true;
				ok = 1;
				// TopRight
				for (int k = 1; k <= 4 && ok; ++k) {
					if (i - k < 0 || j + k >= 15) ok = 0;
					else
						if (b[i - k][j + k] != WHITE) ok = 0;
				}
				if (ok) return true;
				ok = 1;
				// DownRight
				for (int k = 1; k <= 4 && ok; ++k) {
					if (i + k >= 15 || j + k >= 15) ok = 0;
					else
						if (b[i + k][j + k] != WHITE) ok = 0;
				}
				if (ok) return true;
			}
		}
	}
	return false;
}

ll get_board_value(const board& b) {
	ll sum = 0;
	// Traverse all the positions on the board
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (b[i][j] == BLACK) {
				// Top
				for (ll k = 1, s = 0, cnt = 1; check_bound(i - k, j) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i - k][j] == WHITE) break;
					else if (b[i - k][j] == BLACK) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i + 1, j) && check_bound(i - 1, j) && check_bound(i - 4, j)) {
								if (b[i + 1][j] == EMPTY && b[i - 1][j] == EMPTY && b[i - 4][j] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i + 1, j) && check_bound(i - 2, j) && check_bound(i - 4, j)) {
								if (b[i + 1][j] == EMPTY && b[i - 2][j] == EMPTY && b[i - 4][j] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i + 1, j) && check_bound(i - 3, j))
									if (b[i + 1][j] == EMPTY && b[i - 3][j] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i + 2, j)) {
										if (b[i + 2][j] == EMPTY) {
											s *= 9, chk1 = 0;
										}
									}
									if (chk1 && check_bound(i - 4, j)) {
										if (b[i - 4][j] == EMPTY) {
											s *= 9;
										}
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i + 1, j) && check_bound(i - 4, j)) // Live Four
								if (b[i + 1][j] == EMPTY && b[i - 4][j] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum += s;
						else sum -= s;
					}
				}
				// Down
				for (ll k = 1, s = 0, cnt = 1; check_bound(i + k, j) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i + k][j] == WHITE) break;
					else if (b[i + k][j] == BLACK) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i - 1, j) && check_bound(i + 1, j) && check_bound(i + 4, j)) {
								if (b[i - 1][j] == EMPTY && b[i + 1][j] == EMPTY && b[i + 4][j] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i - 1, j) && check_bound(i + 2, j) && check_bound(i + 4, j)) {
								if (b[i - 1][j] == EMPTY && b[i + 2][j] == EMPTY && b[i + 4][j] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i - 1, j) && check_bound(i + 3, j))
									if (b[i - 1][j] == EMPTY && b[i + 3][j] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i - 2, j)) {
										if (b[i - 2][j] == EMPTY) {
											s *= 9, chk1 = 0;
										}
									}
									if (chk1 && check_bound(i + 4, j)) {
										if (b[i + 4][j] == EMPTY) {
											s *= 9;
										}
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i - 1, j) && check_bound(i + 4, j)) // Live Four
								if (b[i - 1][j] == EMPTY && b[i + 4][j] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum += s;
						else sum -= s;
					}
				}
				// Left
				for (ll k = 1, s = 0, cnt = 1; check_bound(i, j - k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i][j - k] == WHITE) break;
					else if (b[i][j - k] == BLACK) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i, j + 1) && check_bound(i, j - 1) && check_bound(i, j - 4)) {
								if (b[i][j + 1] == EMPTY && b[i][j - 1] == EMPTY && b[i][j - 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i, j + 1) && check_bound(i, j - 2) && check_bound(i, j - 4)) {
								if (b[i][j + 1] == EMPTY && b[i][j - 2] == EMPTY && b[i][j - 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i, j + 1) && check_bound(i, j - 3))
									if (b[i][j + 1] == EMPTY && b[i][j - 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i, j + 2)) {
										if (b[i][j + 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i, j - 4)) {
										if (b[i][j - 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i, j + 1) && check_bound(i, j - 4)) // Live Four
								if (b[i][j + 1] == EMPTY && b[i][j - 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum += s;
						else sum -= s;
					}
				}
				// Right
				for (ll k = 1, s = 0, cnt = 1; check_bound(i, j + k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i][j + k] == WHITE) break;
					else if (b[i][j + k] == BLACK) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i, j - 1) && check_bound(i, j + 1) && check_bound(i, j + 4)) {
								if (b[i][j - 1] == EMPTY && b[i][j + 1] == EMPTY && b[i][j + 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i, j - 1) && check_bound(i, j + 2) && check_bound(i, j + 4)) {
								if (b[i][j - 1] == EMPTY && b[i][j + 2] == EMPTY && b[i][j + 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i, j - 1) && check_bound(i, j + 3))
									if (b[i][j - 1] == EMPTY && b[i][j + 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i, j - 2)) {
										if (b[i][j - 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i, j + 4)) {
										if (b[i][j + 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i, j - 1) && check_bound(i, j + 4)) // Live Four
								if (b[i][j - 1] == EMPTY && b[i][j + 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum += s;
						else sum -= s;
					}
				}
				// TopLeft
				for (ll k = 1, s = 0, cnt = 1; check_bound(i - k, j - k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i - k][j - k] == WHITE) break;
					else if (b[i - k][j - k] == BLACK) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i + 1, j + 1) && check_bound(i - 1, j - 1) && check_bound(i - 4, j - 4)) {
								if (b[i + 1][j + 1] == EMPTY && b[i - 1][j - 1] == EMPTY && b[i - 4][j - 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i + 1, j + 1) && check_bound(i - 2, j - 2) && check_bound(i - 4, j - 4)) {
								if (b[i + 1][j + 1] == EMPTY && b[i - 2][j - 2] == EMPTY && b[i - 4][j - 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i + 1, j + 1) && check_bound(i - 3, j - 3))
									if (b[i + 1][j + 1] == EMPTY && b[i - 3][j - 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i + 2, j + 2)) {
										if (b[i + 2][j + 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i - 4, j - 4)) {
										if (b[i - 4][j - 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i + 1, j + 1) && check_bound(i - 4, j - 4)) // Live Four
								if (b[i + 1][j + 1] == EMPTY && b[i - 4][j - 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum += s;
						else sum -= s;
					}
				}
				// TopRight
				for (ll k = 1, s = 0, cnt = 1; check_bound(i - k, j + k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i - k][j + k] == WHITE) break;
					else if (b[i - k][j + k] == BLACK) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i + 1, j - 1) && check_bound(i - 1, j + 1) && check_bound(i - 4, j + 4)) {
								if (b[i + 1][j - 1] == EMPTY && b[i - 1][j + 1] == EMPTY && b[i - 4][j + 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i + 1, j - 1) && check_bound(i - 2, j + 2) && check_bound(i - 4, j + 4)) {
								if (b[i + 1][j - 1] == EMPTY && b[i - 2][j + 2] == EMPTY && b[i - 4][j + 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i + 1, j - 1) && check_bound(i - 3, j + 3))
									if (b[i + 1][j - 1] == EMPTY && b[i - 3][j + 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i + 2, j - 2)) {
										if (b[i + 2][j - 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i - 4, j + 4)) {
										if (b[i - 4][j + 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i + 1, j - 1) && check_bound(i - 4, j + 4)) // Live Four
								if (b[i + 1][j - 1] == EMPTY && b[i - 4][j + 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum += s;
						else sum -= s;
					}
				}
				// DownLeft
				for (ll k = 1, s = 0, cnt = 1; check_bound(i + k, j - k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i + k][j - k] == WHITE) break;
					else if (b[i + k][j - k] == BLACK) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i + 1, j - 1) && check_bound(i - 1, j + 1) && check_bound(i + 4, j - 4)) {
								if (b[i + 1][j - 1] == EMPTY && b[i - 1][j + 1] == EMPTY && b[i + 4][j - 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i - 1, j + 1) && check_bound(i + 2, j - 2) && check_bound(i + 4, j - 4)) {
								if (b[i - 1][j + 1] == EMPTY && b[i + 2][j - 2] == EMPTY && b[i + 4][j - 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i - 1, j + 1) && check_bound(i + 3, j - 3))
									if (b[i - 1][j + 1] == EMPTY && b[i + 3][j - 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i - 2, j + 2)) {
										if (b[i - 2][j + 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i + 4, j - 4)) {
										if (b[i + 4][j - 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i - 1, j + 1) && check_bound(i + 4, j - 4)) // Live Four
								if (b[i - 1][j + 1] == EMPTY && b[i + 4][j - 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum += s;
						else sum -= s;
					}
				}
				// DownRight
				for (ll k = 1, s = 0, cnt = 1; check_bound(i + k, j + k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i + k][j + k] == WHITE) break;
					else if (b[i + k][j + k] == BLACK) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i - 1, j - 1) && check_bound(i + 1, j + 1) && check_bound(i + 4, j + 4)) {
								if (b[i - 1][j - 1] == EMPTY && b[i + 1][j + 1] == EMPTY && b[i + 4][j + 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i - 1, j - 1) && check_bound(i + 2, j + 2) && check_bound(i + 4, j + 4)) {
								if (b[i - 1][j - 1] == EMPTY && b[i + 2][j + 2] == EMPTY && b[i + 4][j + 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i - 1, j - 1) && check_bound(i + 3, j + 3))
									if (b[i - 1][j - 1] == EMPTY && b[i + 3][j + 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i - 2, j - 2)) {
										if (b[i + 2][j + 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i + 4, j + 4)) {
										if (b[i + 4][j + 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i - 1, j - 1) && check_bound(i + 4, j + 4)) // Live Four
								if (b[i - 1][j - 1] == EMPTY && b[i + 4][j + 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum += s;
						else sum -= s;
					}
				}
			}
			else if (b[i][j] == WHITE) {
				// Top
				for (ll k = 1, s = 0, cnt = 1; check_bound(i - k, j) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i - k][j] == BLACK) break;
					else if (b[i - k][j] == WHITE) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i + 1, j) && check_bound(i - 1, j) && check_bound(i - 4, j)) {
								if (b[i + 1][j] == EMPTY && b[i - 1][j] == EMPTY && b[i - 4][j] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i + 1, j) && check_bound(i - 2, j) && check_bound(i - 4, j)) {
								if (b[i + 1][j] == EMPTY && b[i - 2][j] == EMPTY && b[i - 4][j] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i + 1, j) && check_bound(i - 3, j))
									if (b[i + 1][j] == EMPTY && b[i - 3][j] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i + 2, j)) {
										if (b[i + 2][j] == EMPTY) {
											s *= 9, chk1 = 0;
										}
									}
									if (chk1 && check_bound(i - 4, j)) {
										if (b[i - 4][j] == EMPTY) {
											s *= 9;
										}
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i + 1, j) && check_bound(i - 4, j)) // Live Four
								if (b[i + 1][j] == EMPTY && b[i - 4][j] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum -= s;
						else sum += s;
					}
				}
				// Down
				for (ll k = 1, s = 0, cnt = 1; check_bound(i + k, j) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i + k][j] == BLACK) break;
					else if (b[i + k][j] == WHITE) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i - 1, j) && check_bound(i + 1, j) && check_bound(i + 4, j)) {
								if (b[i - 1][j] == EMPTY && b[i + 1][j] == EMPTY && b[i + 4][j] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i - 1, j) && check_bound(i + 2, j) && check_bound(i + 4, j)) {
								if (b[i - 1][j] == EMPTY && b[i + 2][j] == EMPTY && b[i + 4][j] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i - 1, j) && check_bound(i + 3, j))
									if (b[i - 1][j] == EMPTY && b[i + 3][j] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i - 2, j)) {
										if (b[i - 2][j] == EMPTY) {
											s *= 9, chk1 = 0;
										}
									}
									if (chk1 && check_bound(i + 4, j)) {
										if (b[i + 4][j] == EMPTY) {
											s *= 9;
										}
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i - 1, j) && check_bound(i + 4, j)) // Live Four
								if (b[i - 1][j] == EMPTY && b[i + 4][j] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum -= s;
						else sum += s;
					}
				}
				// Left
				for (ll k = 1, s = 0, cnt = 1; check_bound(i, j - k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i][j - k] == BLACK) break;
					else if (b[i][j - k] == WHITE) s *= 9, ++cnt;

					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i, j + 1) && check_bound(i, j - 1) && check_bound(i, j - 4)) {
								if (b[i][j + 1] == EMPTY && b[i][j - 1] == EMPTY && b[i][j - 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i, j + 1) && check_bound(i, j - 2) && check_bound(i, j - 4)) {
								if (b[i][j + 1] == EMPTY && b[i][j - 2] == EMPTY && b[i][j - 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i, j + 1) && check_bound(i, j - 3))
									if (b[i][j + 1] == EMPTY && b[i][j - 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i, j + 2)) {
										if (b[i][j + 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i, j - 4)) {
										if (b[i][j - 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i, j + 1) && check_bound(i, j - 4)) // Live Four
								if (b[i][j + 1] == EMPTY && b[i][j - 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum -= s;
						else sum += s;
					}
				}
				// Right
				for (ll k = 1, s = 0, cnt = 1; check_bound(i, j + k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i][j + k] == BLACK) break;
					else if (b[i][j + k] == WHITE) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i, j - 1) && check_bound(i, j + 1) && check_bound(i, j + 4)) {
								if (b[i][j - 1] == EMPTY && b[i][j + 1] == EMPTY && b[i][j + 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i, j - 1) && check_bound(i, j + 2) && check_bound(i, j + 4)) {
								if (b[i][j - 1] == EMPTY && b[i][j + 2] == EMPTY && b[i][j + 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i, j - 1) && check_bound(i, j + 3))
									if (b[i][j - 1] == EMPTY && b[i][j + 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i, j - 2)) {
										if (b[i][j - 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i, j + 4)) {
										if (b[i][j + 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i, j - 1) && check_bound(i, j + 4)) // Live Four
								if (b[i][j - 1] == EMPTY && b[i][j + 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum -= s;
						else sum += s;
					}
				}
				// TopLeft
				for (ll k = 1, s = 0, cnt = 1; check_bound(i - k, j - k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i - k][j - k] == BLACK) break;
					else if (b[i - k][j - k] == WHITE) s *= 9, ++cnt;

					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i + 1, j + 1) && check_bound(i - 1, j - 1) && check_bound(i - 4, j - 4)) {
								if (b[i + 1][j + 1] == EMPTY && b[i - 1][j - 1] == EMPTY && b[i - 4][j - 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i + 1, j + 1) && check_bound(i - 2, j - 2) && check_bound(i - 4, j - 4)) {
								if (b[i + 1][j + 1] == EMPTY && b[i - 2][j - 2] == EMPTY && b[i - 4][j - 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i + 1, j + 1) && check_bound(i - 3, j - 3))
									if (b[i + 1][j + 1] == EMPTY && b[i - 3][j - 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i + 2, j + 2)) {
										if (b[i + 2][j + 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i - 4, j - 4)) {
										if (b[i - 4][j - 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i + 1, j + 1) && check_bound(i - 4, j - 4)) // Live Four
								if (b[i + 1][j + 1] == EMPTY && b[i - 4][j - 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum -= s;
						else sum += s;
					}
				}
				// TopRight
				for (ll k = 1, s = 0, cnt = 1; check_bound(i - k, j + k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i - k][j + k] == BLACK) break;
					else if (b[i - k][j + k] == WHITE) s *= 9, ++cnt;

					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i + 1, j - 1) && check_bound(i - 1, j + 1) && check_bound(i - 4, j + 4)) {
								if (b[i + 1][j - 1] == EMPTY && b[i - 1][j + 1] == EMPTY && b[i - 4][j + 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i + 1, j - 1) && check_bound(i - 2, j + 2) && check_bound(i - 4, j + 4)) {
								if (b[i + 1][j - 1] == EMPTY && b[i - 2][j + 2] == EMPTY && b[i - 4][j + 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i + 1, j - 1) && check_bound(i - 3, j + 3))
									if (b[i + 1][j - 1] == EMPTY && b[i - 3][j + 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i + 2, j - 2)) {
										if (b[i + 2][j - 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i - 4, j + 4)) {
										if (b[i - 4][j + 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i + 1, j - 1) && check_bound(i - 4, j + 4)) // Live Four
								if (b[i + 1][j - 1] == EMPTY && b[i - 4][j + 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum -= s;
						else sum += s;
					}
				}
				// DownLeft
				for (ll k = 1, s = 0, cnt = 1; check_bound(i + k, j - k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i + k][j - k] == BLACK) break;
					else if (b[i + k][j - k] == WHITE) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i + 1, j - 1) && check_bound(i - 1, j + 1) && check_bound(i + 4, j - 4)) {
								if (b[i + 1][j - 1] == EMPTY && b[i - 1][j + 1] == EMPTY && b[i + 4][j - 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i - 1, j + 1) && check_bound(i + 2, j - 2) && check_bound(i + 4, j - 4)) {
								if (b[i - 1][j + 1] == EMPTY && b[i + 2][j - 2] == EMPTY && b[i + 4][j - 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i - 1, j + 1) && check_bound(i + 3, j - 3))
									if (b[i - 1][j + 1] == EMPTY && b[i + 3][j - 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i - 2, j + 2)) {
										if (b[i - 2][j + 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i + 4, j - 4)) {
										if (b[i + 4][j - 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i - 1, j + 1) && check_bound(i + 4, j - 4)) // Live Four
								if (b[i - 1][j + 1] == EMPTY && b[i + 4][j - 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum -= s;
						else sum += s;
					}
				}
				// DownRight
				for (ll k = 1, s = 0, cnt = 1; check_bound(i + k, j + k) && k <= 4; ++k) {
					if (k == 1) {
						if (player == 1) s = 1;
						else s = 9;
					}
					if (b[i + k][j + k] == BLACK) break;
					else if (b[i + k][j + k] == WHITE) s *= 9, ++cnt;
					if (k == 4) {
						if (cnt == 3) {
							if (check_bound(i - 1, j - 1) && check_bound(i + 1, j + 1) && check_bound(i + 4, j + 4)) {
								if (b[i - 1][j - 1] == EMPTY && b[i + 1][j + 1] == EMPTY && b[i + 4][j + 4] == EMPTY)
									s *= 9;
							}
							else if (check_bound(i - 1, j - 1) && check_bound(i + 2, j + 2) && check_bound(i + 4, j + 4)) {
								if (b[i - 1][j - 1] == EMPTY && b[i + 2][j + 2] == EMPTY && b[i + 4][j + 4] == EMPTY)
									s *= 9;
							}
							else {
								bool chk = 0;
								if (check_bound(i - 1, j - 1) && check_bound(i + 3, j + 3))
									if (b[i - 1][j - 1] == EMPTY && b[i + 3][j + 3] == EMPTY)
										chk = 1;
								if (chk) {
									bool chk1 = 1;
									if (check_bound(i - 2, j - 2)) {
										if (b[i + 2][j + 2] == EMPTY)
											s *= 9, chk1 = 0;
									}
									if (chk1 && check_bound(i + 4, j + 4)) {
										if (b[i + 4][j + 4] == EMPTY)
											s *= 9;
									}
								}
							}
						}
						else if (cnt == 4) {
							s *= 9;
							if (check_bound(i - 1, j - 1) && check_bound(i + 4, j + 4)) // Live Four
								if (b[i - 1][j - 1] == EMPTY && b[i + 4][j + 4] == EMPTY)
									s *= 9;
						}
						else if (cnt == 5) s *= 81;
						if (player == 1) sum -= s;
						else sum += s;
					}
				}
			}
		}
	}
	return sum;
}

board initial_board, record;

ll alpha_beta_pruning(board& b, int depth, ll alpha, ll beta, bool maximizingPlayer) {
	if (!depth || check_win(b))
		return get_board_value(b);
	if (maximizingPlayer) {
		ll val = LLONG_MIN;
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (b[i][j] == EMPTY) {
					b[i][j] = player;
					ll ret = alpha_beta_pruning(b, depth - 1, alpha, beta, false);
					if (ret > val) {
						val = ret;
						if (depth == 2) record = b;
					}
					b[i][j] = EMPTY;
					alpha = max(alpha, val);
					if (alpha >= beta)
						return val;
				}
			}
		}
		return val;
	}
	else {
		ll val = LLONG_MAX;
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (b[i][j] == EMPTY) {
					if (player == 1) b[i][j] = WHITE;
					else if (player == 2) b[i][j] = BLACK;
					ll ret = alpha_beta_pruning(b, depth - 1, alpha, beta, true);
					val = min(val, ret);
					beta = min(beta, val);
					b[i][j] = EMPTY;
					if (beta <= alpha)
						return val;
				}
			}
		}
		return val;
	}
}

void read_board(std::ifstream& fin) {
	fin >> player;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fin >> initial_board[i][j];
		}
	}
}

void write_valid_spot(std::ofstream& fout) {
	//srand(time(NULL));
	alpha_beta_pruning(initial_board, 2, LLONG_MIN, LLONG_MAX, true);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (initial_board[i][j] != record[i][j]) {
				fout << i << " " << j << endl;
				fout.flush();
			}
		}
	}
}

int main(int, char** argv) {
	std::ifstream fin(argv[1]);
	std::ofstream fout(argv[2]);
	read_board(fin);
	write_valid_spot(fout);
	fin.close();
	fout.close();
	return 0;
}
