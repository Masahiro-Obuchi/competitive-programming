#include <bits/stdc++.h>

#include <cassert>
#define rep(i, n) for (int i = 0; i < (n); i++)
using namespace std;
using ll = long long;
using P = pair<int, int>;
struct Edge {
    ll to;
    ll weight;
    Edge(ll t, ll w) : to(t), weight(w) {}
};
using Graph = vector<vector<Edge>>;
// using Graph = vector<vector<int>>;

const long long INF = std::numeric_limits<long long>::max();
const int INT_INF = std::numeric_limits<int>::max();

int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1};  // 右、下、左、上
// int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1}, dy[8] = {-1, 0, 1, 1, -1, 1, 0, -1};

uint64_t get_rand_range(uint64_t min_val, uint64_t max_val) {
    // 乱数生成器
    static std::mt19937_64 mt64(0);

    // [min_val, max_val] の一様分布整数 (int) の分布生成器
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(min_val, max_val);

    // 乱数を生成
    return get_rand_uni_int(mt64);
}

double get_rand_range_real(double min_val, double max_val) {
    // 乱数生成器
    static std::mt19937_64 mt64(0);

    // [min_val, max_val] の一様分布整数 (int) の分布生成器
    std::uniform_real_distribution<double> get_rand_real(min_val, max_val);

    // 乱数を生成
    return get_rand_real(mt64);
}

struct point {
    int x;
    int y;
    point() : x(0), y(0){};
    ~point() = default;
};

struct InputData {
    int N_;  // グラフの頂点数 500 <= N <= 1000
    int M_;  // 辺数 500 <= M <= 3000
    int D_;  // 日数 5 <= D <= 30
    int K_;  // 一日に工事可能な変数の上限 ceil(M/D) <= K <= 2*ceil(M/D)

    vector<tuple<int, int, int>> edges_;
    Graph G_;                         // 各頂点数の次数は2以上である
    Graph halfG_;                     // 有向グラフ
    vector<point> vertexCoordinate_;  // 0 <= xi, yi <= 1000

    map<int, P> edgeNumberToHalfGrahIndex_;
    vector<vector<int>> halfGraphIndexToEdgeNumber_;

    vector<vector<int>>
        uvToGraphEdgeIndex_;  // uvToGraphEdgeIndex_[u][v]でG_[u]の対応する辺のindexが求まる

    vector<int> edgeUsedNumInShortestRoute_;

    InputData(){};
    ~InputData() = default;

    void input() {
        cin >> N_ >> M_ >> D_ >> K_;
        G_.resize(N_);
        halfG_.resize(N_);

        halfGraphIndexToEdgeNumber_.resize(N_);
        uvToGraphEdgeIndex_.resize(N_);
        for (int i = 0; i < N_; i++) {
            halfGraphIndexToEdgeNumber_[i].resize(N_);
            uvToGraphEdgeIndex_[i].resize(N_);
        }

        edgeUsedNumInShortestRoute_.resize(M_);

        for (int i = 0; i < M_; i++) {
            int u, v, w;
            cin >> u >> v >> w;
            u--, v--;
            G_[u].push_back({v, w});
            G_[v].push_back({u, w});

            auto nowGuIndex = G_[u].size() - 1;
            auto nowGvIndex = G_[v].size() - 1;
            uvToGraphEdgeIndex_[u][v] = nowGuIndex;
            uvToGraphEdgeIndex_[v][u] = nowGvIndex;

            if (u < v) {
                halfG_[u].push_back({v, w});
                edgeNumberToHalfGrahIndex_[i] = {u, v};
                halfGraphIndexToEdgeNumber_[u][v] = i;
                halfGraphIndexToEdgeNumber_[v][u] = i;
            } else {
                halfG_[v].push_back({u, w});
                edgeNumberToHalfGrahIndex_[i] = {v, u};
                halfGraphIndexToEdgeNumber_[v][u] = i;
                halfGraphIndexToEdgeNumber_[u][v] = i;
            }

            edges_.push_back({u, v, w});
        }

        vertexCoordinate_.resize(N_);
        for (int i = 0; i < N_; i++) {
            cin >> vertexCoordinate_[i].x >> vertexCoordinate_[i].y;
        }
    };
};

struct OutputData {
    vector<int> r;

    OutputData(){};
    OutputData(int M) { r.resize(M); };
    ~OutputData() = default;

    void output() {
        for (const auto &it : r) {
            cout << it << " ";
        }
        cout << endl;
    }
};

template <typename T>
vector<T> dijkstra(Graph &G, int start) {
    vector<T> dist(G.size(), INT_INF);
    // pair(その時点での最短経路，頂点番号)
    priority_queue<P, vector<P>, greater<P>> que;
    dist[start] = 0;
    que.push(make_pair(dist[start], start));
    while (!que.empty()) {
        int cost = que.top().first;
        int idx = que.top().second;
        que.pop();
        if (dist[idx] < cost) continue;

        for (auto e : G[idx]) {
            if (dist[e.to] <= cost + e.weight) continue;
            dist[e.to] = cost + e.weight;
            que.push(make_pair(dist[e.to], e.to));
        }
    }
    return dist;
}

class AHC017 {
    InputData input_;
    OutputData output_;

    Graph initialGraph_;
    Graph halfInitialGraph_;

  public:
    AHC017(){};
    ~AHC017() = default;

    void solve() {
        input_.input();
        output_.r.resize(input_.M_);

        initialGraph_ = input_.G_;

        computeDistMatrix(0);

        createInitialOutput();

        cerr << "N: " << input_.N_ << "  M: " << input_.M_ << "  K: " << input_.K_
             << "  D: " << input_.D_ << endl;
        // cerr << "now score is : " << computeScore() << endl;
        output_.output();
    };

    bool isInDistanceThreshold(const point &p1, const point &p2) {
        static constexpr double vertexDistanceThreshold = 100000.;
        return sqrt(pow(p1.x - p2.x, 2.) + pow(p1.y - p2.y, 2.)) < vertexDistanceThreshold;
    };

    // create very simple output
    void solution1() {
        int cycle = (input_.M_ + input_.D_ - 1) / input_.D_;
        for (int day = 1; day <= input_.D_; day++) {
            int nowEdge = day - 1;
            for (int k = 0; k < input_.K_; k++) {
                if (nowEdge >= input_.M_) continue;
                output_.r[nowEdge] = day;
                nowEdge += input_.D_;
            }
        }
    }

    // 辺集中　失敗
    void solution2() {
        vector<bool> isProcessed(input_.M_, false);
        vector<int>
            vertexOnProcess;  // 処理中の頂点番号 この頂点の残り辺が1本かそれ以外かで場合分け
        vector<int> vertexEdgeIndex(input_.N_);
        set<int> remainedVertex;
        for (int i = 0; i < input_.N_; i++) remainedVertex.insert(i);

        for (int day = 1; day <= input_.D_; day++) {
            int dayIndex = day - 1;
            // 関数として切り出せそうなところは切り出す
            int rem = (input_.M_ + input_.D_ - 1) / input_.D_;
            if (day == input_.D_) {
                for (int i = 0; i < input_.M_; i++) {
                    if (!isProcessed[i]) output_.r[i] = day;
                }
                break;
            }

            if (vertexOnProcess.size() > 0) {
                auto itr = vertexOnProcess.begin();
                while (itr != vertexOnProcess.end()) {
                    auto u = *itr;
                    fillEdgeOfVertex(vertexOnProcess, vertexEdgeIndex, isProcessed,
                                     input_.halfG_[u].size(), u, day, rem);
                    if (vertexEdgeIndex[u] == input_.halfG_[u].size())
                        itr = vertexOnProcess.erase(itr);
                    else
                        itr++;
                    if (rem == 0) break;
                }
            }
            if (rem > 0) {
                // vertexOnProcessへの追加、vertexEdgeIndexのインクリメント
                if (remainedVertex.size()) {
                    int u = choiseRandomVertex(vertexEdgeIndex);
                    if (remainedVertex.find(u) != remainedVertex.end()) remainedVertex.erase(u);
                    if (input_.halfG_[u].size()) vertexOnProcess.push_back(u);
                    // TODO: 焼きなましで頂点間の距離の2乗和の最大化
                    // 辺を1本だけ残す　上のwhile文の中身と共通化出来そう
                    while (rem > 0) {
                        if (input_.halfG_[u].size()) {
                            fillEdgeOfVertex(vertexOnProcess, vertexEdgeIndex, isProcessed,
                                             input_.halfG_[u].size() - 1, u, day, rem);
                        }
                        if (rem == 0) break;
                        // 次の頂点に移動する
                        if (remainedVertex.empty()) {
                            break;
                        }
                        u = choiseNextVertex(vertexEdgeIndex, u);
                        if (remainedVertex.find(u) != remainedVertex.end()) remainedVertex.erase(u);
                        if (input_.halfG_[u].size()) vertexOnProcess.push_back(u);
                    }
                }
            }
        }
    }

    int choiseRandomVertex(const vector<int> &vertexEdgeIndex) {
        int u = get_rand_range(0, input_.N_ - 1);
        while (vertexEdgeIndex[u] > 0) {
            u = get_rand_range(0, input_.N_ - 1);
        }
        return u;
    }

    int choiseRandomEdge(const vector<bool> &isEdgeProcessed) {
        int e = get_rand_range(0, input_.M_ - 1);
        while (isEdgeProcessed[e]) {
            e = get_rand_range(0, input_.M_ - 1);
        }
        return e;
    }

    double calculateDistanceTwoEdges(int edgeIndex1, int edgeIndex2) {
        auto [u1, v1] = input_.edgeNumberToHalfGrahIndex_[edgeIndex1];
        auto [u2, v2] = input_.edgeNumberToHalfGrahIndex_[edgeIndex2];

        double edgeIndex1CenterX =
            (input_.vertexCoordinate_[u1].x + input_.vertexCoordinate_[v1].x) / 2.0;
        double edgeIndex1CenterY =
            (input_.vertexCoordinate_[u1].y + input_.vertexCoordinate_[v1].y) / 2.0;
        double edgeIndex2CenterX =
            (input_.vertexCoordinate_[u2].x + input_.vertexCoordinate_[v2].x) / 2.0;
        double edgeIndex2CenterY =
            (input_.vertexCoordinate_[u2].y + input_.vertexCoordinate_[v2].y) / 2.0;

        return sqrt(pow(edgeIndex1CenterX - edgeIndex2CenterX, 2.0) +
                    pow(edgeIndex1CenterY - edgeIndex2CenterY, 2.0));
    }

    double calculateSumOfDistanceTwoEdges(const vector<int> &edgeSet) {
        double dist = 0.0;
        for (int i = 0; i < edgeSet.size(); i++) {
            for (int j = i + 1; j < edgeSet.size(); j++) {
                dist += calculateDistanceTwoEdges(edgeSet[i], edgeSet[j]);
            }
        }
        return dist;
    }

    int choiseRandomEdge(const vector<bool> &isEdgeProcessed, int prevEdge) {
        int e = get_rand_range(0, input_.M_ - 1);
        while (isEdgeProcessed[e] /* || calculateDistanceTwoEdges(e, prevEdge) > 10000 */) {
            e = get_rand_range(0, input_.M_ - 1);
            // cerr << "e: " << e << "   bbbb" << endl;
        }
        return e;
    }

    int choiseNextVertex(const vector<int> &vertexEdgeIndex, int prevu) {
        int nxtu = get_rand_range(0, input_.N_ - 1);
        while (vertexEdgeIndex[nxtu] > 0) {
            nxtu = get_rand_range(0, input_.N_ - 1);
        }
        return nxtu;
    }

    void fillEdgeOfVertex(vector<int> &vertexOnProcess, vector<int> &vertexEdgeIndex,
                          vector<bool> &isProcessed, int fillLimit, int u, int day, int &rem) {
        int processedEdgeCount = 0;
        while (rem > 0 && vertexEdgeIndex[u] < fillLimit) {
            auto &ei = vertexEdgeIndex[u];
            auto v = input_.halfG_[u][ei].to;
            auto edgeIndex = input_.halfGraphIndexToEdgeNumber_[u][v];
            output_.r[edgeIndex] = day;
            isProcessed[edgeIndex] = true;
            ei++;
            rem--;
            processedEdgeCount++;
        }
    }

    void processRandomEdge(vector<bool> &isEdgeProcessed, vector<vector<int>> &edgeSetPerDay,
                           set<int> &notProcessedEdge, vector<vector<vector<bool>>> &cEdgeInGraph,
                           int day) {
        int e = choiseRandomEdge(isEdgeProcessed);
        isEdgeProcessed[e] = true;
        output_.r[e] = day;
        edgeSetPerDay[day - 1].push_back(e);
        auto [u, v] = input_.edgeNumberToHalfGrahIndex_[e];

        auto nowGuIndex = input_.uvToGraphEdgeIndex_[u][v];
        auto nowGvIndex = input_.uvToGraphEdgeIndex_[v][u];
        cEdgeInGraph[day - 1][u][nowGuIndex] = true;
        cEdgeInGraph[day - 1][v][nowGvIndex] = true;

        if (notProcessedEdge.find(e) != notProcessedEdge.end()) notProcessedEdge.erase(e);
    }

    bool isReachableVertex(const vector<vector<vector<bool>>> &cEdgeInGraph, int day, int u) {
        bool ok = false;
        for (auto v : cEdgeInGraph[day][u]) {
            if (!v) ok = true;
        }
        return ok;
    }

    int countUnreachableVertexNumber(const vector<vector<vector<bool>>> &cEdgeGraph) {
        int unreachableVertexCount = 0;
        for (int day = 0; day < input_.D_; day++) {
            for (int u = 0; u < input_.N_; u++) {
                if (!isReachableVertex(cEdgeGraph, day, u)) unreachableVertexCount++;
            }
        }
        return unreachableVertexCount;
    }

    ll calcAverageUsedEdgeNum(int totalDay) {
        ll ave = 0;
        for (const auto it : input_.edgeUsedNumInShortestRoute_) {
            ave += it;
        }
        ave /= totalDay;
        return ave;
    }

    ll calcAverageUsedEdgeNumMulWeight(int totalDay) {
        ll ave = 0;
        for (int i = 0; i < input_.M_; i++) {
            ll weight = get<2>(input_.edges_[i]);
            ave += input_.edgeUsedNumInShortestRoute_[i] * weight;
        }
        ave /= totalDay;
        return ave;
    }

    ll computeSecondItem(const vector<vector<int>> &edgeSetPerDay, ll averageUsedEdgeNumMulWeight,
                         ll averageUsedEdgeNum) {
        ll res = 0;
        for (int day = 0; day < input_.D_; day++) {
            ll sum = 0;
            for (auto eIdx : edgeSetPerDay[day]) {
                ll weight = get<2>(input_.edges_[eIdx]);
                sum += input_.edgeUsedNumInShortestRoute_[eIdx] * weight;
            }
            res += (averageUsedEdgeNumMulWeight - sum) / 100000 *
                   (averageUsedEdgeNumMulWeight - sum) / 1000000;
        }
        return res;
    }

    ll computeSecondItem(const vector<vector<int>> &edgeSetPerDay, ll averageUsedEdgeNum) {
        ll res = 0;
        for (int day = 0; day < input_.D_; day++) {
            ll sum = 0;
            for (auto eIdx : edgeSetPerDay[day]) {
                ll weight = get<2>(input_.edges_[eIdx]);
                sum += input_.edgeUsedNumInShortestRoute_[eIdx];
            }
            res += (averageUsedEdgeNum - sum) / 10 * (averageUsedEdgeNum - sum) / 100;
        }
        return res;
    }

    ll computeSimulatedAnnealingScore(vector<vector<vector<bool>>> &cEdgeInGraph,
                                      vector<vector<int>> &edgeSetPerDay,
                                      vector<double> sumOfDistanceTwoEdgesPerDay,
                                      ll averageUsedEdgeNum, ll averageUsedEdgeNumMulWeight) {
        ll unreachableVertexCount = countUnreachableVertexNumber(cEdgeInGraph);
        ll l2DistUsedEdgeNum = computeSecondItem(edgeSetPerDay, averageUsedEdgeNum);
        /*
        ll l2DistUsedEdgeNumMulWeight =
            computeSecondItem(edgeSetPerDay, averageUsedEdgeNumMulWeight, averageUsedEdgeNum);
        */
        ll sumOfDistanceTwoEdges = 0;
        for (auto it : sumOfDistanceTwoEdgesPerDay) {
            sumOfDistanceTwoEdges += it;
        }

        ll alpha = 1, beta = 1;
        /*
        cerr << "first: " << unreachableVertexCount * 10000000000 << endl;
        cerr << "second: " << l2DistUsedEdgeNumMulWeight << endl;
        cerr << "second 2: " << l2DistUsedEdgeNum << endl;
        cerr << "Third: " << sumOfDistanceTwoEdges << endl;
        */
        return -unreachableVertexCount * 10000000000 - alpha * l2DistUsedEdgeNum +
               beta * sumOfDistanceTwoEdges;
        /*
 return -unreachableVertexCount * 10000000000 - alpha * l2DistUsedEdgeNumMulWeight;
return -unreachableVertexCount * 10000000000 - alpha * l2DistUsedEdgeNum;
        */
    }

    void createInitialOutput() {
        // 辺をランダムに選んで、その内1本を使用して次の頂点へ
        // 前回選んだ辺から距離の制限を設ける

        vector<vector<int>> edgeSetPerDay(input_.D_);  // 各日の辺集合 setにするかは要検討
        vector<vector<vector<bool>>> cEdgeInGraph(
            input_.D_, vector<vector<bool>>(input_.N_));  // cEdgeInGraph[day][u][v] = true/false
                                                          // 頂点が到達可能かどうかの判定に使用する
        for (int day = 0; day < input_.D_; day++) {
            for (int u = 0; u < input_.N_; u++) {
                cEdgeInGraph[day][u].resize(input_.G_[u].size());
            }
        }

        vector<bool> isEdgeProcessed(input_.M_);
        set<int> notProcessedEdge;
        for (int i = 0; i < input_.M_; i++) notProcessedEdge.insert(i);
        for (int day = 1; day <= input_.D_; day++) {
            int rem = (input_.M_ + input_.D_ - 1) / input_.D_;
            while (rem > 0 && !notProcessedEdge.empty()) {
                processRandomEdge(isEdgeProcessed, edgeSetPerDay, notProcessedEdge, cEdgeInGraph,
                                  day);
                rem--;
            }
        }

        ll averageUsedEdgeNum = calcAverageUsedEdgeNum(input_.D_);
        ll averageUsedEdgeNumMulWeight = calcAverageUsedEdgeNumMulWeight(input_.D_);
        vector<double> sumOfDistanceTwoEdgesPerDay(input_.D_);
        for (int day = 0; day < input_.D_; day++) {
            sumOfDistanceTwoEdgesPerDay[day] = calculateSumOfDistanceTwoEdges(edgeSetPerDay[day]);
        }

        cerr << "before compute simulated annealing score: "
             << computeSimulatedAnnealingScore(cEdgeInGraph, edgeSetPerDay,
                                               sumOfDistanceTwoEdgesPerDay, averageUsedEdgeNum,
                                               averageUsedEdgeNumMulWeight)
             << endl;
        cerr << "unreachable vertex before: " << countUnreachableVertexNumber(cEdgeInGraph) << endl;

        simulatedAnnealing(cEdgeInGraph, edgeSetPerDay, sumOfDistanceTwoEdgesPerDay,
                           averageUsedEdgeNum, averageUsedEdgeNumMulWeight);

        cerr << "after compute simulated annealing score: "
             << computeSimulatedAnnealingScore(cEdgeInGraph, edgeSetPerDay,
                                               sumOfDistanceTwoEdgesPerDay, averageUsedEdgeNum,
                                               averageUsedEdgeNumMulWeight)
             << endl;
        cerr << "unreachable vertex after: " << countUnreachableVertexNumber(cEdgeInGraph) << endl;
        /*
        for (auto it : output_.r) cerr << it << " ";
        cerr << endl;
        */
        for (int day = 2; day < 3; day++) {
            cerr << "day: " << day << endl;
            for (int i = 46; i < 48; i++) {
                cerr << "u: " << i << endl;
                for (auto v : cEdgeInGraph[day][i]) {
                    cerr << v << " ";
                }
                cerr << endl;
            }
        }
    };

    void updateSumOfDistanceTwoEdgesPerDay(vector<vector<int>> &edgeSetPerDay,
                                           vector<double> &sumOfDistanceTwoEdgesPerDay,
                                           int dayIndex1, int dayIndex2, int edgeIndex1,
                                           int edgeIndex2) {
        for (int i = 0; i < edgeSetPerDay[dayIndex1].size(); i++) {
            sumOfDistanceTwoEdgesPerDay[dayIndex1] -=
                calculateDistanceTwoEdges(edgeSetPerDay[dayIndex1][i], edgeIndex1);
            sumOfDistanceTwoEdgesPerDay[dayIndex1] +=
                calculateDistanceTwoEdges(edgeSetPerDay[dayIndex1][i], edgeIndex2);
        }
        for (int i = 0; i < edgeSetPerDay[dayIndex2].size(); i++) {
            sumOfDistanceTwoEdgesPerDay[dayIndex2] -=
                calculateDistanceTwoEdges(edgeSetPerDay[dayIndex2][i], edgeIndex1);
            sumOfDistanceTwoEdgesPerDay[dayIndex2] +=
                calculateDistanceTwoEdges(edgeSetPerDay[dayIndex2][i], edgeIndex2);
        }
    }

    void simulatedAnnealing(vector<vector<vector<bool>>> &cEdgeInGraph,
                            vector<vector<int>> &edgeSetPerDay,
                            vector<double> &sumOfDistanceTwoEdgesPerDay, ll averageUsedEdgeNum,
                            ll averageUsedEdgeNumMulWeight) {
        const double TIME_LIMIT = 5.5;
        auto startTime = chrono::system_clock::now();
        double startTemp = 5000, endTemp = 100;
        double nowTime = 0.0;

        int simulatingCnt = 0;
        while (nowTime < TIME_LIMIT) {
            const double progressRatio = nowTime / TIME_LIMIT;
            const double temp = startTemp + (endTemp - startTemp) * progressRatio;

            simulatingCnt++;

            ll oldScore = computeSimulatedAnnealingScore(
                cEdgeInGraph, edgeSetPerDay, sumOfDistanceTwoEdgesPerDay, averageUsedEdgeNum,
                averageUsedEdgeNumMulWeight);

            // 近傍を定義
            // ランダムにある日に処理する辺と別の日に処理する辺を入れ替える
            const int dayIndex1 = get_rand_range(0, input_.D_ - 1);
            const int dayIndex2 = get_rand_range(0, input_.D_ - 1);
            if (dayIndex1 == dayIndex2) continue;
            const int edgeIndexDay1 = get_rand_range(0, edgeSetPerDay[dayIndex1].size() - 1);
            const int edgeIndexDay2 = get_rand_range(0, edgeSetPerDay[dayIndex2].size() - 1);

            // TODO: cEdgeInGraph, sumOfDistanceTwoEdgesPerDay, edgeSetPerDay,
            // output_.rの変更とrevert output_.rで見たときのedgeのindex
            const int edge1 = edgeSetPerDay[dayIndex1][edgeIndexDay1];
            const int edge2 = edgeSetPerDay[dayIndex2][edgeIndexDay2];
            // cEdgeInGraph
            const auto [u1, v1] = input_.edgeNumberToHalfGrahIndex_[edge1];
            const auto nowGuIndex1 = input_.uvToGraphEdgeIndex_[u1][v1];
            const auto nowGvIndex1 = input_.uvToGraphEdgeIndex_[v1][u1];

            const auto [u2, v2] = input_.edgeNumberToHalfGrahIndex_[edge2];
            const auto nowGuIndex2 = input_.uvToGraphEdgeIndex_[u2][v2];
            const auto nowGvIndex2 = input_.uvToGraphEdgeIndex_[v2][u2];

            cEdgeInGraph[dayIndex1][u1][nowGuIndex1] = !cEdgeInGraph[dayIndex1][u1][nowGuIndex1];
            cEdgeInGraph[dayIndex1][v1][nowGvIndex1] = !cEdgeInGraph[dayIndex1][v1][nowGvIndex1];
            cEdgeInGraph[dayIndex1][u2][nowGuIndex2] = !cEdgeInGraph[dayIndex1][u2][nowGuIndex2];
            cEdgeInGraph[dayIndex1][v2][nowGvIndex2] = !cEdgeInGraph[dayIndex1][v2][nowGvIndex2];
            cEdgeInGraph[dayIndex2][u1][nowGuIndex1] = !cEdgeInGraph[dayIndex2][u1][nowGuIndex1];
            cEdgeInGraph[dayIndex2][v1][nowGvIndex1] = !cEdgeInGraph[dayIndex2][v1][nowGvIndex1];
            cEdgeInGraph[dayIndex2][u2][nowGuIndex2] = !cEdgeInGraph[dayIndex2][u2][nowGuIndex2];
            cEdgeInGraph[dayIndex2][v2][nowGvIndex2] = !cEdgeInGraph[dayIndex2][v2][nowGvIndex2];

            // sumOfDistanceTwoEdgesPerDay
            ll oldDay1SumOfDistanceTwoEdgesPerDay = sumOfDistanceTwoEdgesPerDay[dayIndex1];
            ll oldDay2SumOfDistanceTwoEdgesPerDay = sumOfDistanceTwoEdgesPerDay[dayIndex2];
            updateSumOfDistanceTwoEdgesPerDay(edgeSetPerDay, sumOfDistanceTwoEdgesPerDay, dayIndex1,
                                              dayIndex2, edge1, edge2);
            // edgeSetPerDay
            swap(edgeSetPerDay[dayIndex1][edgeIndexDay1], edgeSetPerDay[dayIndex2][edgeIndexDay2]);
            // output_.r
            swap(output_.r[edge1], output_.r[edge2]);

            ll newScore = computeSimulatedAnnealingScore(
                cEdgeInGraph, edgeSetPerDay, sumOfDistanceTwoEdgesPerDay, averageUsedEdgeNum,
                averageUsedEdgeNumMulWeight);
            ;
            double deltaScore = (double)newScore - oldScore;
            double prob = exp(deltaScore / temp);
            if (prob > get_rand_range_real(0.0, 1.0)) {
                // 受容
            } else {
                // 棄却　元の状態に戻す
                // cEdgeInGraph
                cEdgeInGraph[dayIndex1][u1][nowGuIndex1] =
                    !cEdgeInGraph[dayIndex1][u1][nowGuIndex1];
                cEdgeInGraph[dayIndex1][v1][nowGvIndex1] =
                    !cEdgeInGraph[dayIndex1][v1][nowGvIndex1];
                cEdgeInGraph[dayIndex1][u2][nowGuIndex2] =
                    !cEdgeInGraph[dayIndex1][u2][nowGuIndex2];
                cEdgeInGraph[dayIndex1][v2][nowGvIndex2] =
                    !cEdgeInGraph[dayIndex1][v2][nowGvIndex2];
                cEdgeInGraph[dayIndex2][u1][nowGuIndex1] =
                    !cEdgeInGraph[dayIndex2][u1][nowGuIndex1];
                cEdgeInGraph[dayIndex2][v1][nowGvIndex1] =
                    !cEdgeInGraph[dayIndex2][v1][nowGvIndex1];
                cEdgeInGraph[dayIndex2][u2][nowGuIndex2] =
                    !cEdgeInGraph[dayIndex2][u2][nowGuIndex2];
                cEdgeInGraph[dayIndex2][v2][nowGvIndex2] =
                    !cEdgeInGraph[dayIndex2][v2][nowGvIndex2];
                // sumOfDistanceTwoEdgesPerDay
                sumOfDistanceTwoEdgesPerDay[dayIndex1] = oldDay1SumOfDistanceTwoEdgesPerDay;
                sumOfDistanceTwoEdgesPerDay[dayIndex2] = oldDay2SumOfDistanceTwoEdgesPerDay;
                // edgeSetPerDay
                swap(edgeSetPerDay[dayIndex1][edgeIndexDay1],
                     edgeSetPerDay[dayIndex2][edgeIndexDay2]);
                // output_.r
                swap(output_.r[edge1], output_.r[edge2]);
            }

            nowTime = (chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() -
                                                                   startTime)
                           .count() *
                       1e-6);
        }
        cerr << "Simulate count: " << simulatingCnt << endl;
    }

    template <typename T>
    vector<T> dijkstraDay0(Graph &G, vector<int> &prev, int start) {
        // input_.edgeUsedNumInShortestRoute_
        vector<T> dist(G.size(), INT_INF);
        prev.resize(G.size(), -1);
        // pair(その時点での最短経路，頂点番号)
        priority_queue<P, vector<P>, greater<P>> que;
        dist[start] = 0;
        que.push(make_pair(dist[start], start));
        while (!que.empty()) {
            int cost = que.top().first;
            int idx = que.top().second;
            que.pop();
            if (dist[idx] < cost) continue;
            for (auto e : G[idx]) {
                if (dist[e.to] <= cost + e.weight) continue;
                dist[e.to] = cost + e.weight;
                prev[e.to] = idx;  // 頂点idxからe.toへ
                que.push(make_pair(dist[e.to], e.to));
            }
        }
        return dist;
    }

    // t: ゴール
    void countUsedNumInDijkstra(const vector<int> &prev, int t) {
        // input_.edgeUsedNumInShortestRoute_に対して加算していく
        int prevVertex = t;
        for (int curVertex = prev[t]; curVertex != -1; curVertex = prev[curVertex]) {
            int edgeIndex = input_.halfGraphIndexToEdgeNumber_[curVertex][prevVertex];
            input_.edgeUsedNumInShortestRoute_[edgeIndex]++;
            prevVertex = curVertex;
        }
        return;
    }

    vector<vector<ll>> computeDistMatrix(int day) {
        vector<vector<ll>> distMatrix;
        if (day == 0) {
            for (int i = 0; i < input_.N_; i++) {
                vector<int> prev;
                vector<ll> nowDist = dijkstraDay0<ll>(initialGraph_, prev, i);
                distMatrix.push_back(nowDist);
                for (int t = 0; t < input_.N_; t++) {
                    countUsedNumInDijkstra(prev, t);
                }
            }
            /*
            for (auto it : input_.edgeUsedNumInShortestRoute_) cerr << it << " ";
            cerr << endl;
            */
        } else {
            Graph nowGraph = initialGraph_;
            for (int e = 0; e < input_.M_; e++) {
                // 辺の重みを変化させる
                if (output_.r[e] == day) {
                    // このときのuとvを調べる
                    P constructionVertex = {std::get<0>(input_.edges_[e]),
                                            std::get<1>(input_.edges_[e])};  // {u, v}
                    for (auto &constructionEdge : nowGraph[constructionVertex.first]) {
                        if (constructionEdge.to == constructionVertex.second) {
                            constructionEdge.weight = 1000000000LL;
                        }
                    }
                    for (auto &constructionEdge : nowGraph[constructionVertex.second]) {
                        if (constructionEdge.to == constructionVertex.first) {
                            constructionEdge.weight = 1000000000LL;
                        }
                    }
                }
            }
            // この状態でdistMatrixを作成する
            for (int i = 0; i < input_.N_; i++) {
                vector<ll> nowDist = dijkstra<ll>(nowGraph, i);
                distMatrix.push_back(nowDist);
            }
        }
        return distMatrix;
    };

    ll computeScore() {
        ll fkSum = 0;
        vector<vector<ll>> initialDistMatrix = computeDistMatrix(0);

        for (int day = 1; day <= input_.D_; day++) {
            vector<vector<ll>> nowDistMatrix = computeDistMatrix(day);
            ll tmp = 0;
            for (int i = 0; i < input_.N_; i++) {
                for (int j = i + 1; j < input_.N_; j++) {
                    tmp += nowDistMatrix[i][j] - initialDistMatrix[i][j];
                }
            }
            fkSum += tmp;
        }

        long double numer = fkSum;
        long double denom = input_.D_ * input_.N_ * (input_.N_ - 1) / 2.;
        long double average = numer / denom * 1000.;
        return static_cast<ll>(round(average));
    };
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    AHC017 ahc017;
    ahc017.solve();

    return 0;
}
