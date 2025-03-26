기본 템플릿이 블루프린트로 되어있어서 기능 담당하는 부분만 c++ 라이브러리로 자성하여 블루프린트로 이어줬습니다.


GetRandom()
GetRandom은 TArray<int32>형식의 3개의 숫자배열을 생성하는 로직입니다.
1부터 9까지 1개씩 들어있는 배열을 생성 후,
세 차례에 걸쳐 랜덤 인덱스의 숫자를 추출해서 result 배열에 저장하고 원래 배열에서 제거합니다.
vector에서 remove는 맨 뒤에 인덱스가 사라지지 않지만 tarray는 사라집니다.

ConvertToNumber(FString StringInput)
ConvertToNumber은 사용자로부터 입력받은 FString에서 TArray<int32>를 추출하는 함수입니다.
TArray 자료구조를 잘 몰라서 입력받은 FString을 string으로 전환합니다.
/가 위치한 인덱스를 찾고 (=it)
substr로 it+1부터 3개의 string을 추출 후
각 char마다 '0'을 빼주어서 숫자로 변환하고 TArray<int32>에 Add해주고 return해줍니다.

FGameResult
FGameResult는 bool bIsAnswer, int32 BallCount, int32 StrikeCount, int32 OutCount로 구성된 구조체입니다.
JudgeNumbers에서 모든 값을 한꺼번에 반환하기 위해 만들었습니다.

JudgeNumbers(const TArray<int32>& SecretNumber, const TArray<int32>& PlayerNumber)
JudgeNumbers는 먼저 내부에서 FGameResult 객체를 생성하고, 판단 후에 그 객체를 반환합니다.
입력값 둘 중에 하나라도 비어있으면 초기값에서 바로 반환하고,
for문을 거쳐서 같은 인덱스에 같은 값이 있으면 strikecount++
그렇지 않고 포함만 한다면 ballcount++
둘 다 아니라면 outcount++
strikecount가 3이라면 true를 반환합니다.

ToFStringFromTArray(const TArray<int32>& PlayerNumber)
ToFStringFromTArray는 ConverToNumbers를 출력하기 위해 FString으로 변환 후 반환하는 함수입니다.
