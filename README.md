# Whisper of Forest
![](https://cdn.jsdelivr.net/gh/m2za5/Whisper_of_Forest@main/Image.Png)

타락한 숲을 정화하고 정령들을 구해내는 3D 오픈월드 액션 어드벤처 게임입니다.

## 게임 소개

성년을 맞이한 엘프 주인공이 타락한 숲의 정령들을 정화하며 숲이 죽어가는 원인을 추적하는 이야기입니다. 플레이어는 숲 곳곳에 봉인된 정령왕의 제단을 찾아 봉인을 해제하고, 타락한 정령왕과 전투한 뒤 정화합니다. 정령왕이 정화될 때마다 숲의 자연이 회복되며, 정화된 정령들과 상호작용할 수 있게 됩니다.

## 기술 스택

- **Engine:** Unreal Engine 5
- **Language:** C++
- **버전 관리:** Git / GitHub
- **협업 도구:** Notion

## 팀 구성

6인 팀 프로젝트

## 담당 파트

### 맵 제작 및 최적화
- 오픈월드 맵 레벨 디자인 (마을, 숲길, 타락한 숲 구역)
- `stat unit` / `stat fps` 기반 성능 프로파일링
- LOD 적용, Actor Merge, Shadow Cull, 폴리지 Cull Distance 설정
- 최적화 결과: FPS 35 → 64 (82% 향상), Prims 4,021K → 1,439K (64% 감소)

### 제단 정화 시스템
- 제단 봉인 해제 → 정령왕 스폰 → 전투 → 정화 연출까지의 전체 흐름 구현
- 정화 시 포스트 프로세스 볼륨을 활용한 밝기 변화, 안개 제거, VFX(빛줄기, 반딧불이) 생성
- 나뭇잎 알베도 색상 변경 및 Fresnel 효과를 활용한 타락/정화 상태 시각 표현
- 기능별 컴포넌트 분리 리팩토링 (AltarEffectComponent, AltarTimelineComponent, AltarCameraComponent)

### 컷신 매니저
- `UWorldSubsystem` 기반 컷신 관리 시스템
- `ULevelSequencePlayer`를 활용한 컷신 재생, 일시정지, 재개, 스킵 기능
- 컷신 순차 재생 및 종료 콜백을 통한 정화 연출 연동
- CineCameraActor를 활용한 시점 전환

### 세계수 포탈 시스템
- 세계수 해금 및 해금된 세계수 간 텔레포트 기능
- `WorldTreeManager`를 통한 해금 상태 관리
- 텔레포트 후 오버랩 재활성화 타이머를 활용한 무한 트리거 방지
- 포탈 UI 위젯 구현 (해금된 세계수 목록 표시 및 선택 텔레포트)

### 체인 물리 시뮬레이션
- 제단 봉인 해제 시 사슬이 물리적으로 풀리는 연출 구현
- Merged Static Mesh에서 TriSel을 활용해 체인 영역만 분리
- 개별 체인 고리에 Simulate Physics 및 Physics Constraint Component 적용

### 기타
- 머티리얼 제작 및 수정 (Emissive Mask, 알베도 조정 등)
- VFX 제작 (안개, 빛줄기, 반딧불이 등)

## 주요 스크립트

| 파일 | 설명 |
|------|------|
| `AltarPurification` | 제단 정화 전체 로직 관리 (컴포넌트 초기화 및 호출) |
| `AltarEffectComponent` | 안개, 빛줄기, 반딧불이 등 VFX 생성 및 제어 |
| `AltarTimelineComponent` | 밝기 타임라인 및 정화 완료 이벤트 관리 |
| `AltarCameraComponent` | 컷신 카메라 전환 및 복원 |
| `AltarPurifyComponent` | 정령왕 사망 감지 후 컷신 재생 및 정화 트리거 |
| `CutsceneManager` | 컷신 재생/정지/스킵/상태 관리 (UWorldSubsystem) |
| `CutsceneFinishedProxy` | 컷신 종료 이벤트 브릿지 |
| `WorldTree` | 세계수 오버랩 감지, 해금, 텔레포트 |
| `WorldTreeManager` | 해금된 세계수 목록 관리 |
| `WorldTreeWidget` | 포탈 UI 위젯 (세계수 목록 표시 및 텔레포트 실행) |
| `UnlockStone` | 봉인석 상호작용 및 컷신 연동 후 제단 해금 |
| `UnlockAltar` | 제단 해금 및 보스 스폰 |
