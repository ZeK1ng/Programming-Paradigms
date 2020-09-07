#include <bits/stdc++.h>
using namespace std;
#define ZePair pair<int,int>
#define pb push_back
#define scanVec(vec , n) for(int i = 0; i < n ; i++){ cin>>vec[i];} 
#define printVec(vec , n) for(int i = 0; i < n ; i++){ cout<<vec[i]<<" ";} cout<<endl;
#define S second
#define F first

void solve(){
	int s,m;
	cin>>s>>m;
	string str;
	cin>>str;
	vector<int> vec(m);
	for(int i=0; i<m; i++){
		cin>>vec[i];
		vec[i]--;
	}
	vector<int> res(26);
	
	int k =m;
	sort(vec.begin(), vec.end());
	for(int i=0 ;i<str.length();i++){
		res[str[i]-'a']++;
		while(vec.size() && vec.back()<i){
			vec.pop_back();
			k--;
		}
		res[str[i]-'a']+=k;
	}
	
	printVec(res,26);
}

int main(){
    ios::sync_with_stdio(0); 
    cin.tie(0);
    cout.tie(0);
    int k ;
    cin>>k;
    while(k--)
    solve();
    return 0;
}
