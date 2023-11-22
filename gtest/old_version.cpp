// #pragma once
// #include <bits/stdc++.h>
// namespace correct {
// // int n;
// // int efficiency[502][502];
// // vector<vector<int>> g;
// // std::vector<int> correspondence;
// // std::vector<bool> used;
// int border;
// std::vector<bool> block;
// int e[1003][1003];
// int ans[503];
// // int parents[1003];
//
// bool DFS(std::vector<std::vector<int>>& left_connections,
//          std::vector<bool>& used,
//          std::vector<int>& correspondence,
//          int node) {
//   if (used[node]) return false;
//   used[node] = true;
//   for (int i = 0; i < left_connections[node].size(); i++) {
//     int to = left_connections[node][i];
//     if (correspondence[to] == -1
//         || DFS(left_connections,
//                used,
//                correspondence,
//                correspondence[to])) {
//       correspondence[to] = node;
//       return true;
//     }
//   }
//   return false;
// }
//
// bool FindCorrespondence(std::vector<std::vector<int>>& left_connections,
//                         std::vector<int>& correspondence,
//                         int node) {
//   std::vector<bool> used(left_connections.size(), false);
//   return DFS(left_connections, used, correspondence, node);
// }
//
// std::optional<std::vector<int>> Good(std::vector<std::vector<int>>& efficiency,
//                                      int min_efficiency) {
//   std::vector<std::vector<int>> left_connections(efficiency.size());
//   for (int i = 0; i < efficiency.size(); i++) {
//     for (int j = 0; j < efficiency[i].size(); j++) {
//       if (efficiency[i][j] >= min_efficiency) {
//         left_connections[i].push_back(j);
//       }
//     }
//   }
//
//   std::vector<int> correspondence(efficiency.size(), -1);
//   for (int i = 0; i < efficiency.size(); ++i) {
//     FindCorrespondence(left_connections, correspondence, i);
//   }
//   int successfully_found = 0;
//   for (int i = 0; i < efficiency.size(); ++i) {
//     if (correspondence[i] != -1) successfully_found++;
//   }
//   if (successfully_found != efficiency.size()) {
//     return std::nullopt;
//   }
//   return correspondence;
// }
//
// void DFS(std::vector<bool>& used, std::vector<int>& parents,
//          int node, int prev_node, int n) {
//   if (used[node]) return;
//   used[node] = true;
//   parents[node] = prev_node;
//   for (int i = 0; i < 2 * n; i++) {
//     if (e[node][i] == 1 && !block[i]) {
//       DFS(used, parents, i, node, n);
//     }
//   }
// }
//
// void findLex(std::vector<std::vector<int>>& efficiency, int v) {
//   int n = (int) efficiency.size();
//   for (int i = 0; i < n; i++) e[i][ans[i] + n] = 1;
//   std::vector<bool> used(2 * n, false);
//   std::vector<int> parents(2 * n, -1);
//   DFS(used, parents, v, v, efficiency.size());
//   for (int i = 0; i < n; i++) e[i][ans[i] + n] = 0;
//   for (int i = n; i < 2 * n; i++) {
//     if (used[i] && !block[i] && efficiency[v][i - n] >= border) {
//       block[i] = true;
//       ans[v] = i - n;
//       int u = parents[i];
//       while (u != parents[u]) {
//         ans[u] = parents[u] - n;
//         u = parents[parents[u]];
//       }
//       break;
//     }
//   }
//   block[v] = true;
// }
//
// std::vector<int> ModifyAnswer(std::vector<std::vector<int>>& efficiency,
//                      const std::vector<int>& correspondence) {
//   int n = efficiency.size();
//   block.assign(2 * n, false);
//   for (int i = 0; i < n; i++) ans[correspondence[i]] = i;
//   for (int i = 0; i < n; i++) {
//     for (int j = 0; j < n; j++) {
//       if (efficiency[i][j] >= border) e[n + j][i] = 1;
//     }
//   }
//   for (int i = 0; i < n; i++) {
//     findLex(efficiency, i);
//   }
//
//   std::vector<int> result(n);
//   for (int i = 0; i < n; ++i) {
//     result[i] = ans[i] + 1;
//   }
//   return result;
// }
//
// std::vector<int> GetAnswer(const std::vector<int>& correspondence) {
//   std::vector<int> answer(correspondence.size());
//   for (int i = 0; i < correspondence.size(); ++i) {
//     answer[correspondence[i]] = i + 1;
//   }
//   return answer;
// }
//
// std::pair<int, std::vector<int>> CorrectSolve(int n,
//                                               const std::vector<std::vector<int>>& efficiency_copy) {
//   int max_efficiency = 0;
//   std::vector<std::vector<int>> efficiency(n, std::vector<int>(n));
//   for (int i = 0; i < n; i++) {
//     for (int j = 0; j < n; j++) {
//       efficiency[i][j] = efficiency_copy[i][j];
//       max_efficiency = std::max(max_efficiency, efficiency[i][j]);
//     }
//   }
//
//   int l = 0;
//   int r = max_efficiency + 1;
//   std::vector<int> result_correspondence;
//   while (r > l + 1) {
//     int mid = (l + r) >> 1;
//     auto current_correspondence = Good(efficiency, mid);
//     if (current_correspondence.has_value()) {
//       l = mid;
//       result_correspondence = current_correspondence.value();
//     } else {
//       r = mid;
//     }
//   }
//   border = l;
//   return {border, ModifyAnswer(efficiency, result_correspondence)};
// }
//
// }