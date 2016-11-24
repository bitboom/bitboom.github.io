---
layout: post
title:  Floyd-Warshall Algorithm
tags: [algorithm] 
---

# [플로이드-워셜 알고리즘(Floyd-Warshall Algorithm)](https://ko.wikipedia.org/wiki/%ED%94%8C%EB%A1%9C%EC%9D%B4%EB%93%9C-%EC%9B%8C%EC%85%9C_%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98)
> 그래프에서 **모든 꼭짓점 사이의 최단 경로의 거리를 구하는 알고리즘**이다.  
> 음수 가중치를 갖는 간선도 순환만 없다면 잘 처리된다. *-> non-negative cycle*  
> 제일 바깥쪽 반복문은 거쳐가는 꼭짓점이고, 두 번째 반복문은 출발하는 꼭짓점,  
> 세 번째 반복문은 도착하는 꼭짓점이다.

- 복잡도 O(n^3)으로 다익스트라 알고리즘(dijkstra)과 같으나 **코드 구현이 간단**
- 3중 for문으로 구현 되며 제일 바깥쪽이 경유지 k가 됨
- 대표적 동적 계획 알고리즘    

## KEY IDEA
- 모든 쌍(출발지, 도착지)에 대하여 경유지 k를 거쳐서 가는 길이 빠른지 검사함
- 모든 점을 경유 가능한 점들로 고려
- 모든 경유지를 고려하기에, 경유지의 순서는 상관 없음 (최소값으로 renewal) 


## BASE CODE
```
void FloydWarshall() {
    for (int k = 1; k <= N; k++) {
        for (int i = 1; i <= N; i++) {
            if (i == k) continue;        
            for (int j = 1; j <= N; j++) {
                if (j == k || i == j) continue;
                DP[i][j] = (DP[i][j] < DP[i][k] + DP[k][j]) ? DP[i][j] : DP[i][k] + DP[k][j];
            }
        }
    }
}
``` 

## ADVANCED
- 출발지에서 경유지 또는 경유지에서 목적지가 고려 대상이 아닐 때 (길이 없을 때)  
  기존 DP[i][j]값이 유지 됨으로 계산 할 필요 없음.

```
// 최적화
void FloydWarshall() {
    for (int k = 1; k <= N; k++) {
        for (int i = 1; i <= N; i++) {
            if (i == k) continue;        
            if (DP[i][k] == MAX_TIME) continue;
            for (int j = 1; j <= N; j++) {
                if (j == k || i == j) continue;
                if (DP[k][j] == MAX_TIME) continue;
                DP[i][j] = (DP[i][j] < DP[i][k] + DP[k][j]) ? DP[i][j] : DP[i][k] + DP[k][j];
            }
        }
    }
}
``` 

## PRACTICE
<https://www.acmicpc.net/problem/1238>
