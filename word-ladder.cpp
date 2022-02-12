#include <string>
#include <vector>
#include <stack>
#include <iostream>

using namespace std;

//Andrew's solution 1: https://leetcode.com/submissions/detail/640124748/
//Andrew's solution 2: https://leetcode.com/submissions/detail/639924438/

class Solution {
private:
    const size_t NOT_FOUND = -1; //good placeholder I guess, though introduces a strange (and very unlikely) edge case
    inline bool isLink(const string &s1, const string &s2) {
        bool foundDiff = false;
        for (size_t i=0; i<s1.size(); i++)
            if (s1[i]!=s2[i]) {
                if (foundDiff)
                    return false;
                else
                    foundDiff = true;
            }
        return foundDiff;
    }
    struct waypoint {
        size_t index;//index of the word in wordList
        size_t iteration;//for storing current iteration of loop through that word's connections
    };
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        //find where endWord is in the list
        //and also figure out where beginWord is, if it is
        size_t endIndex = NOT_FOUND, beginIndex = NOT_FOUND;
        for (size_t i=0; i<wordList.size(); i++) {
            if (!endWord.compare(wordList[i])) {
                endIndex = i;
                if (beginIndex!=NOT_FOUND)
                    break;
            } else if (!beginWord.compare(wordList[i])) {
                beginIndex = i;
                if (endIndex!=NOT_FOUND)
                    break;
            }
        }
        if (endIndex==NOT_FOUND)
            return 0; // is not in the list
        //otherwise continue on
        stack<size_t> firstBranch;
        for (size_t i=0; i<wordList.size(); i++)
            if (isLink(beginWord, wordList[i]))
                firstBranch.push(i);
        if (firstBranch.size()==0)
            return 0; //if I can't reach any other words from the first one, then there's no path
        //now construct branches overall
        vector<size_t>* branches = new vector<size_t>[wordList.size()];
        //TODO: maybe optimize by only constructing branches that are connected to beginning word and end word
        for (size_t x=0; x<wordList.size(); x++) {
            if (x==beginIndex)
                continue;
            for (size_t y=0; y<wordList.size(); y++)
                if (x!=y && y!=beginIndex && isLink(wordList[x], wordList[y]))
                    branches[x].push_back(y);
        }
        if (branches[endIndex].size()==0)
            return 0;//might as well check here, if endIndex isn't connected to anything then there's no path
        //now iterate over possible branches
        size_t minLenFound = NOT_FOUND;
        stack<waypoint> path;
        vector<bool> used(wordList.size(), false);
        while (!firstBranch.empty()) {
            if (firstBranch.top()==endIndex) {
                //then the path length is only 1, can't beat that... might as well return now
                delete[] branches;
                return 1+1;//forgot, they expect the number to include the first link
            }
            if (minLenFound>2) {
                path.push({firstBranch.top(), 0});
                used[path.top().index] = true;
                while (!path.empty()) {
                    size_t current = path.top().index;//just for readability; this is the index in wordList of the word we're currently on
                    size_t i = path.top().iteration;//we're on current word's ith connection in branches
                    if (i<branches[current].size()) {
                        size_t nextIndex = branches[current][i];
                        if (nextIndex==endIndex) {
                            //we've found a path, check if it's better than the best one found so far
                            //if (path.size()+1<minLenFound) {//actually, is this comparison even necessary? The below might mean it's not
                                minLenFound = path.size()+1;
                                //now pop it off the stack, no point exploring further down this tree in any other direction
                                used[current] = false;
                                path.pop();
                            //}
                        } else if (used[nextIndex]) {//don't descend in if already been down that path
                            path.top().iteration++;
                        } else if (path.size()+2<minLenFound) {//check if traversing the branches would mean going deeper than (or equally deep to) current best found path
                            path.top().iteration++;//will be on nextIndexiteration next time I descend down to here in the tree
                            path.push({nextIndex, 0});
                            used[nextIndex] = true;
                        } else
                            path.top().iteration++;
                    } else {//we've iterated through all of connections to current word, so pop it off the stack
                        used[current] = false;
                        path.pop();
                    }
                }
            }
            firstBranch.pop();
        }
        delete[] branches;
        if (minLenFound==NOT_FOUND)
            return 0;
        return (int)minLenFound+1;//forgot they were including the start word...
    }
};


int main() {
    //vector<string> wordList = {"hot","dot","dog","lot","log","cog"};"cet"
    //vector<string> wordList = {"hot","dot","dog","lot","log","cog"};
    vector<string> wordList = {"kid","tag","pup","ail","tun","woo","erg","luz","brr","gay","sip","kay","per","val","mes","ohs","now","boa","cet","pal","bar","die","war","hay","eco","pub","lob","rue","fry","lit","rex","jan","cot","bid","ali","pay","col","gum","ger","row","won","dan","rum","fad","tut","sag","yip","sui","ark","has","zip","fez","own","ump","dis","ads","max","jaw","out","btu","ana","gap","cry","led","abe","box","ore","pig","fie","toy","fat","cal","lie","noh","sew","ono","tam","flu","mgm","ply","awe","pry","tit","tie","yet","too","tax","jim","san","pan","map","ski","ova","wed","non","wac","nut","why","bye","lye","oct","old","fin","feb","chi","sap","owl","log","tod","dot","bow","fob","for","joe","ivy","fan","age","fax","hip","jib","mel","hus","sob","ifs","tab","ara","dab","jag","jar","arm","lot","tom","sax","tex","yum","pei","wen","wry","ire","irk","far","mew","wit","doe","gas","rte","ian","pot","ask","wag","hag","amy","nag","ron","soy","gin","don","tug","fay","vic","boo","nam","ave","buy","sop","but","orb","fen","paw","his","sub","bob","yea","oft","inn","rod","yam","pew","web","hod","hun","gyp","wei","wis","rob","gad","pie","mon","dog","bib","rub","ere","dig","era","cat","fox","bee","mod","day","apr","vie","nev","jam","pam","new","aye","ani","and","ibm","yap","can","pyx","tar","kin","fog","hum","pip","cup","dye","lyx","jog","nun","par","wan","fey","bus","oak","bad","ats","set","qom","vat","eat","pus","rev","axe","ion","six","ila","lao","mom","mas","pro","few","opt","poe","art","ash","oar","cap","lop","may","shy","rid","bat","sum","rim","fee","bmw","sky","maj","hue","thy","ava","rap","den","fla","auk","cox","ibo","hey","saw","vim","sec","ltd","you","its","tat","dew","eva","tog","ram","let","see","zit","maw","nix","ate","gig","rep","owe","ind","hog","eve","sam","zoo","any","dow","cod","bed","vet","ham","sis","hex","via","fir","nod","mao","aug","mum","hoe","bah","hal","keg","hew","zed","tow","gog","ass","dem","who","bet","gos","son","ear","spy","kit","boy","due","sen","oaf","mix","hep","fur","ada","bin","nil","mia","ewe","hit","fix","sad","rib","eye","hop","haw","wax","mid","tad","ken","wad","rye","pap","bog","gut","ito","woe","our","ado","sin","mad","ray","hon","roy","dip","hen","iva","lug","asp","hui","yak","bay","poi","yep","bun","try","lad","elm","nat","wyo","gym","dug","toe","dee","wig","sly","rip","geo","cog","pas","zen","odd","nan","lay","pod","fit","hem","joy","bum","rio","yon","dec","leg","put","sue","dim","pet","yaw","nub","bit","bur","sid","sun","oil","red","doc","moe","caw","eel","dix","cub","end","gem","off","yew","hug","pop","tub","sgt","lid","pun","ton","sol","din","yup","jab","pea","bug","gag","mil","jig","hub","low","did","tin","get","gte","sox","lei","mig","fig","lon","use","ban","flo","nov","jut","bag","mir","sty","lap","two","ins","con","ant","net","tux","ode","stu","mug","cad","nap","gun","fop","tot","sow","sal","sic","ted","wot","del","imp","cob","way","ann","tan","mci","job","wet","ism","err","him","all","pad","hah","hie","aim","ike","jed","ego","mac","baa","min","com","ill","was","cab","ago","ina","big","ilk","gal","tap","duh","ola","ran","lab","top","gob","hot","ora","tia","kip","han","met","hut","she","sac","fed","goo","tee","ell","not","act","gil","rut","ala","ape","rig","cid","god","duo","lin","aid","gel","awl","lag","elf","liz","ref","aha","fib","oho","tho","her","nor","ace","adz","fun","ned","coo","win","tao","coy","van","man","pit","guy","foe","hid","mai","sup","jay","hob","mow","jot","are","pol","arc","lax","aft","alb","len","air","pug","pox","vow","got","meg","zoe","amp","ale","bud","gee","pin","dun","pat","ten","mob"};
    Solution sol;
    cout << sol.ladderLength("cet", "ism", wordList) << endl;
}
