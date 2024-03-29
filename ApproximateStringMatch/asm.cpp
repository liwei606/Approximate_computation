#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int slibno = 100;
const int sinno = 10;
const int soutno = 10;

class node
{
public:
	int dis;
	int index;
	node *next;
	
	node(int d, int i, node* p): dis(d), index(i), next(p) {}
};

string slib[slibno];
string input[sinno];
string output[soutno];

node *head = new node(-1, -1, NULL);
int curr_sout = 0;

int min(int a, int b, int c) {
	if (a < b) {
		if (c < a) return c;
		else return a;
	}
	if (c < b) return c;
	else return b;
}

int d[50][50];

void clear() {
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			d[i][j] = 0;
		}
	}
}

int edit_dis(string a, string b) {
	int la = a.length();
	int lb = b.length();
	clear();
	for (int i = 0; i <= la; ++i) {
		d[i][0] = i;
	}
	for (int i = 0; i <= lb; ++i) {
		d[0][i] = i;
	}
	for (int i = 1; i <= la; ++i) {
		for (int j = 1; j <= lb; ++j) {
			// __asm__()
			if (a[i-1] == b[j-1] || a[i-1] == b[j-1] + 'A' - 'a' ||  a[i-1] + 'A' - 'a' == b[j-1]) d[i][j] = d[i - 1][j - 1];
			else d[i][j] = min(
								d[i-1][j] + 1, 
								d[i][j-1] + 1, 
								d[i-1][j-1] + 1
							);
			
			// __asm__()
		}
	}
	return d[la][lb];
}

void release() {
	node *temp = head->next;
	if (temp) {
		head -> next = temp -> next;
		delete temp;
	}
}

void replace(int curr_edis, int curr_index) {
	node *p = NULL;
	bool minimal;
	if (curr_sout < 10) {
		if (curr_sout > 0) {
			p = head;
			minimal = true;
			for (; p->next; p = p->next) {
				if (curr_edis > p->next->dis) {
					p->next = new node(curr_edis, curr_index, p->next);
					minimal = false;
					break;
				}
			}
			if (minimal) {
				p->next = new node(curr_edis, curr_index, NULL);
			}
		}
		else head->next = new node(curr_edis, curr_index, NULL);
		++curr_sout;
		return;
	}
	
	if (curr_edis > head->next->dis) return;
	minimal = true;
	p = head;
	for (; p->next; p = p->next) {
		if (curr_edis > p->next->dis) {
			p->next = new node(curr_edis, curr_index, p->next);
			release();
			minimal = false;
			break;
		}
	}
	if (minimal) {
		p->next = new node(curr_edis, curr_index, NULL);
		release();
	}
}

void match(string s) {
	int curr_edis;
	for (int i = 0; i < slibno; ++i) {
		curr_edis = edit_dis(s, slib[i]);
		replace(curr_edis, i);
	}
	node *q = head->next;
	for (int i = 9; i >= 0 && q; --i) {
		output[i] = slib[q->index];
		release();
		q = q->next;
		--curr_sout;
	}
}

int main() {
	freopen("slib.txt", "r", stdin);
	for (int i = 0; i < slibno; ++i) {
		cin >> slib[i];
	}
	fclose(stdin);

	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	for (int i = 0; i < sinno; ++i) {
		cin >> input[i];
		match(input[i]);
		for (int j = 0; j < soutno; ++j) {
			cout << output[j] << endl;
		}
		cout << endl;
	}
	fclose(stdin);
	fclose(stdout);
	delete head;
	return 0;	
}
