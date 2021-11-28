# othello-game
uncomplete othello game

## 추가해야할 사항
1. Player 객체.
2. GameManager 객체의 int plate**; 활용과 턴이 끝날때 말을 뒤집는 알고리즘.
3. UI

## 알려진 오류
1. 보드사이즈를 선택하기전 SIGINT가 전달되고 Cancel을 누를때 보드사이즈 선택 메세지가 안뜨는 현상.

    -> 메세지를 stdscr의 subwin으로 바꾸면 해결이 가능할듯.

2. 점수계산이 2자리수 밖에 안되기 때문에 10x10, 12x12 보드에서는 오버플로우가 일어날 가능성이 있음.
    
    -> 1xx의 점수는 1의 사이즈를 줄여서 출력하고(7x7->5x7) 3자리수를 출력할 수 있게 개선해야 함.
