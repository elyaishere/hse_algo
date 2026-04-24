// Профессору Форду необходимо попасть на международную конференцию.
// Он хочет потратить на дорогу наименьшее количество денег, поэтому решил,
// что будет путешествовать исключительно ночными авиарейсами (чтобы не
// тратиться на ночевку в отелях), а днем будет осматривать
// достопримечательности тех городов, через которые он будет проезжать
// транзитом. Он внимательно изучил расписание авиаперелетов и составил набор
// подходящих авиарейсов, выяснив, что перелеты на выбранных направлениях
// совершаются каждую ночь и за одну ночь он не сможет совершить два перелета.

// Теперь профессор хочет найти путь наименьшей стоимости, учитывая что до
// конференции осталось K ночей (то есть профессор может совершить не более K
// перелетов).

// Входные данные
// В первой строке находятся числа 2<=N<=100 (количество городов),
// 1<=M<=10^5(количество авиарейсов), 1<=K<=100 (количество оставшихся ночей),
// 1<=S<=N (номер города, в котором живет профессор), 1<=F<=N (номер города, в
// котором проводится конференция).

// Далее идет M строк, задающих расписание авиарейсов.
// i-я строка содержит три натуральных числа: Si, Fi и Pi,
// где 1<=Si<=N - номер города, из которого вылетает i-й рейс,
// 1<=Fi<=N - номер города, в который прилетает i-й рейс,
// 1<=Pi<=10^6 - стоимость перелета i-м рейсом.

#include <climits>
#include <iostream>
#include <tuple>
#include <vector>

#define INF INT_MAX

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int num_of_cities, num_of_flights, max_nights, start, finish;
  std::cin >> num_of_cities >> num_of_flights >> max_nights >> start >> finish;

  std::vector<std::tuple<int, int, int>> all_flights;
  for (int i = 0; i < num_of_flights; ++i) {
    int first, second, cost;
    std::cin >> first >> second >> cost;
    all_flights.emplace_back(first - 1, second - 1, cost);
  }

  std::vector<std::vector<int>> dist(max_nights + 1);
  for (int night = 0; night < max_nights + 1; ++night) {
    for (int city = 0; city < num_of_cities; ++city) {
      if (night == 0 && city == start - 1) {
        dist[night].push_back(0);
      } else {
        dist[night].push_back(INF);
      }
    }
  }

  // Bellman–Ford
  for (;;) {
    bool was_relaxed = false;
    for (int night = 1; night < max_nights + 1; ++night) {
      for (auto &flight : all_flights) {
        int u, v, w;
        std::tie(u, v, w) = flight;
        if (dist[night - 1][u] != INF) {
          if (dist[night][v] > dist[night - 1][u] + w) {
            dist[night][v] = dist[night - 1][u] + w;
            was_relaxed = true;
          }
        }
      }
    }
    if (!was_relaxed) {
      break;
    }
  }

  int min_cost = INF;
  for (int night = 0; night <= max_nights; night++) {
    if (dist[night][finish - 1] < min_cost) {
      min_cost = dist[night][finish - 1];
    }
  }

  std::cout << (min_cost == INF ? -1 : min_cost) << std::endl;

  return 0;
}
