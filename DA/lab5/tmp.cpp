#include <bits/stdc++.h>

struct SuffixTree {
    struct Edge {
        int start; 
        std::shared_ptr<int> end; 
        int destination;

        Edge(int s, std::shared_ptr<int> e, int d) : start(s), end(e), destination(d) {}
    };

    std::string text; 
    std::vector< std::vector< Edge > > adjacencyList; 
    std::vector<int> suffixLinks; 
    std::vector<int> depth;

    int findEdge(int node, char c) { 
        for (size_t i = 0; i < adjacencyList[node].size(); ++i) {
            if (c == text[adjacencyList[node][i].start]) {
                return i;
            }
        }
        return -1;
    }

    SuffixTree(const std::string& input) : text(input), adjacencyList(input.size() * 2), suffixLinks(input.size() * 2), depth(input.size() * 2) {
        bool isNewNode = false;
        int idx = 0;
        std::shared_ptr<int> endPtr(new int);
        *endPtr = 0;
        int commonLength = 0;
        int currentMatchLength = 0;
        int currentNode = 0;
        int currentEdge = -1;
        int newNode = 0;

        for (size_t i = 0; i < input.size(); ++i) {
            ++*endPtr;
            int lastCreatedNode = 0;
            int currentCreatedNode = -1;
            while (idx < *endPtr) {
                if (currentEdge == -1) {
                    int nextEdge = findEdge(currentNode, text[idx + commonLength + currentMatchLength]);
                    if (nextEdge == -1) {
                        isNewNode = true;
                    } else {
                        currentMatchLength = 1;
                    }
                    currentEdge = nextEdge;
                } else {
                    if (text[adjacencyList[currentNode][currentEdge].start + currentMatchLength] == text[idx + currentMatchLength + commonLength]) {
                        ++currentMatchLength;
                    } else {
                        isNewNode = true;
                    }
                }
                if (currentMatchLength > 0 && adjacencyList[currentNode][currentEdge].start + currentMatchLength == *adjacencyList[currentNode][currentEdge].end) {
                    currentNode = adjacencyList[currentNode][currentEdge].destination;
                    currentEdge = -1;
                    commonLength = commonLength + currentMatchLength;
                    currentMatchLength = 0;
                }
                if (isNewNode) {
                    ++newNode;
                    if (currentMatchLength == 0 && currentEdge == -1) {
                        adjacencyList[currentNode].push_back(Edge(idx + commonLength, endPtr, newNode));
                    } else {
                        currentCreatedNode = newNode;
                        Edge edge = adjacencyList[currentNode][currentEdge];

                        std::shared_ptr<int> newEndPtr(new int);
                        *newEndPtr = adjacencyList[currentNode][currentEdge].start + currentMatchLength;
                        adjacencyList[currentNode][currentEdge].end = newEndPtr;
                        adjacencyList[currentNode][currentEdge].destination = newNode;
                        edge.start = *newEndPtr;

                        adjacencyList[newNode].push_back(edge);
                        ++newNode;
                        adjacencyList[newNode - 1].push_back(Edge(idx + currentMatchLength + commonLength, endPtr, newNode));
                        depth[newNode - 1] = depth[currentNode] + *adjacencyList[currentNode][currentEdge].end - adjacencyList[currentNode][currentEdge].start;

                        if (lastCreatedNode > 0) {
                            suffixLinks[lastCreatedNode] = currentCreatedNode;
                        }
                        lastCreatedNode = currentCreatedNode;
                    }
                    int suffixLinkNode = suffixLinks[currentNode];
                    int nextEdge = -1;
                    int nextCommonLength = depth[suffixLinkNode];
                    int nextMatchLength = currentMatchLength + commonLength - nextCommonLength - 1;
                    while (nextMatchLength > 0) {
                        nextEdge = findEdge(suffixLinkNode, text[idx + nextCommonLength + 1]);
                        int edgeLength = *adjacencyList[suffixLinkNode][nextEdge].end;
                        int edgeStart = adjacencyList[suffixLinkNode][nextEdge].start;
                        if (edgeLength - edgeStart <= nextMatchLength) {
                            nextCommonLength = nextCommonLength + edgeLength - edgeStart;
                            nextMatchLength = nextMatchLength - edgeLength + edgeStart;
                            suffixLinkNode = adjacencyList[suffixLinkNode][nextEdge].destination;
                            nextEdge = -1;
                        } else {
                            break;
                        }
                    }
                    if (nextEdge != -1) {
                        currentMatchLength = nextMatchLength;
                    } else {
                        currentMatchLength = 0;
                    }
                    commonLength = nextCommonLength;
                    currentNode = suffixLinkNode;
                    currentEdge = nextEdge;
                    ++idx;
                    isNewNode = false;
                } else {
                    if (i < input.size() - 1) {
                        break;
                    }
                }
            }
        }
    }

    std::vector<size_t> findMatches(const std::string& query) {
        size_t queryLength = query.size();
        std::vector<size_t> result(queryLength);
        int currentNode = 0;
        int currentEdge = findEdge(0, query[0]);
        int commonLength = 0;
        int currentMatchLength = 0;
        std::stack<int> nodeStack;
        nodeStack.push(0);
        for (size_t i = 0; i < queryLength; ++i) {
            int nextNode = currentNode;
            while (suffixLinks[nextNode] == 0 && !nodeStack.empty()) {
                nextNode = nodeStack.top();
                nodeStack.pop();
            }
            currentNode = suffixLinks[nextNode];
            while (!nodeStack.empty()) {
                nodeStack.pop();
            }
            nodeStack.push(currentNode);
            currentMatchLength = std::max(0, commonLength + currentMatchLength - 1 - depth[currentNode]);
            commonLength = depth[currentNode];
            currentEdge = -1;
            while (currentMatchLength >= 0) {
                int nextEdge = findEdge(currentNode, query[i + commonLength]);
                if (nextEdge != -1) {
                    int edgeLength = *adjacencyList[currentNode][nextEdge].end - adjacencyList[currentNode][nextEdge].start;
                    if (currentMatchLength >= edgeLength) {
                        currentMatchLength -= edgeLength;
                        commonLength += edgeLength;
                        currentNode = adjacencyList[currentNode][nextEdge].destination;
                        nextEdge = -1;
                    } else {
                        currentEdge = nextEdge;
                        break;
                    }
                } else {
                    break;
                }
            }
            while (currentEdge != -1 && i + commonLength + currentMatchLength < queryLength && query[i + commonLength + currentMatchLength] == text[adjacencyList[currentNode][currentEdge].start + currentMatchLength]) {
                ++currentMatchLength; 
                if (adjacencyList[currentNode][currentEdge].start + currentMatchLength == *adjacencyList[currentNode][currentEdge].end) {
                    int nextNode = adjacencyList[currentNode][currentEdge].destination;
                    int nextEdge = findEdge(nextNode, query[i + commonLength + currentMatchLength]);
                    int edgeLength = currentMatchLength;
                    commonLength += edgeLength;
                    currentMatchLength = 0;
                    currentNode = nextNode;
                    nodeStack.push(currentNode);
                    currentEdge = nextEdge;
                }
            }
            result[i] = commonLength + currentMatchLength;
        }
        return result;
    }
};

int main() {
    std::string pattern, text;
    std::cin >> pattern >> text;
    pattern += "$"; 
    SuffixTree tree(pattern);
    std::vector<size_t> matches(text.size());
    matches = tree.findMatches(text);

    size_t maxMatch = *std::max_element(matches.begin(), matches.end()); 
    std::cout << maxMatch << '\n';
    std::set<std::string> uniqueMatches;

    for (size_t i = 0; i < matches.size(); ++i) {
        if (matches[i] == maxMatch) {
            std::string substring = text.substr(i, maxMatch);
            uniqueMatches.insert(substring);
        }
    }

    for (const auto& match : uniqueMatches) {
        std::cout << match << '\n';
    }

    return 0;
}
