#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ASCII_art.h"

//캐릭터 및 몹 구조체
typedef struct character
{
    //이름
    char name[50];
    int basic_skill;
    /*고유 스킬:
        검사->  1. 베기     ->   4.참격
        궁사->  2. 활쏘기    ->  5.삼연 활쏘기
        마법사->3. 매직미사일 ->  6. 파이어볼

        적들 스킬:
        강아지-> 7. 물기
        케로베로스-> 8.많이 물기
        독수리-> 9.날개치기
    */
    int random_skill;
    /* 랜덤 스킬:
        1. 회복
        2. 방패막
        3. 공격 상승
        4. 기절시키기
        5. 스턴
        6. mp 상승

        적들은 부가 스킬: 
        강아지-> 7. 짓기 & 10. 꼬리 흔들기
        케로베로스-> 8. 웅크리기 & 11. 삼연 핥기
        독수리-> 9. 날아오르기 & 12. 돌풍일으키기 & 13. 깃털 갈무리
    */
    int hp,atk,def,mp;
    // 체력,공격력,방어력,마력
    int potion;
    //포션 개수
    int state;
    /*
        디버프 및 버프상태
        1. 기절: 2턴 행동 불가
        2. 스턴: 1턴 행동 불가 + 2턴 약한 도트뎀
        3. 공격 상승: 3턴 공격력 상승
        4.  mp 상승: 3턴 mp 상승
        5. 방패막: 3턴 def 상승
    */
   int mob;
    /*
        몹종류:
        1.강아지
        2.케로베로스
        3.독수리
    */
}ch;


// 도스 창 크기 변경 및 실행 이름 바꾸는 함수
void setup_console(){    
    //도스의 이름을 바꾸어줌
    system("title RandomRPG");
    //도스의 크기를 고정함
    system("mode con: cols=150 lines=35");
}
//오프닝 함수
void Start_setup(){
    //오프닝 아트
    print_start_graphic();
    Sleep(300);
    //엔터 누르면 다음 이벤트
    printf("\n시작하실라면 엔터를 눌러주세요.");
    getchar();
    system("cls");
}
//튜터리얼(게임 설명) 함수
void Start_basic(){    
    //조작법 설명
    printf("\n\n조작법: 선택은 숫자를 입력해 조작합니다. 확인과 취소는 각각 y와n으로 결정합니다.\n");
    printf("행동 또한 마찬가지며 행동은 공격,방어,회복아이템 사용이 있습니다.\n");
    printf("회복아이템 사용은 총 2회 가능하며 이벤트를 통해 얻을 수 있습니다. \n");
    printf("엔터 다음....");
    getchar();
    system("cls");
    //포션 사용시 설명
    potion_print();
    printf("물약 사용시 체력이 20만큼 회복됩니다.\n");
    printf("엔터 다음....");
    getchar();
    system("cls");
    //방어 선택시 설명
    defense_print();
    printf("방어 사용시 적의 공격을 방어력만큼 막습니다.\n");
    printf("만약 방어로 피해를 다 막는다면 적의 공격은 무효됩니다.\n");
    printf("엔터 다음....");
    getchar();
    system("cls");
    //공격 선택시 설명
    sword_1_attack_print();
    printf("공격 사용시 가지고 있는 두개의 스킬을 중 하나를 선택하게 되며 고유 스킬과 교환 가능한 랜덤 스킬이 있습니다.\n");
    printf("그 후 각 스킬에 맞게 효과가 나타납니다.\n");
    printf("엔터 다음....");
    getchar();
    system("cls");
    //적과 만남시 이벤트 설명
    printf("적과 만난다면 출현 이벤트와 함께 적과 싸우게 됩니다.\n");
    printf("엔터 다음....");
    getchar();
    system("cls");
    spawn_mob_event();
    print_basic_mob();
    printf("이런식으로 말이죠....");
    printf("엔터 다음....");
    getchar();
    system("cls");
    printf("그럼 행운을 빕니다! 캐릭터를 생성하세요!(계속할라면 엔터...)\n");
    getchar();
    system("cls");
}
//강아지 몹 만드는 함수
ch *enemy_1(){
    ch *enemy = (ch *)malloc(sizeof(ch));
    //이름 설정
    strcpy(enemy->name,"강아지");
    //스탯 설정
    enemy->hp=90;
    enemy->atk=20;
    enemy->def=20;
    enemy->mp=10;
    //기술 설정
    enemy->basic_skill=7;
    enemy->random_skill=7;
    //버프&디버프설정
    enemy->state=0;
    //몹종류를 알려줌
    enemy->mob=1;
    return enemy;
}

//캐릭터 만드는 이벤트
ch *character_making(){
    ch *new_character = (ch *)malloc(sizeof(ch));
    char name[50];
    char check ='n';
    int job;
    //이름 설정
    printf("이름을 적어주세요.\n");
    printf("이름: ");
    scanf("%s",name);
    strcpy(new_character->name,name);
    //기본 포션 두개 설정
    new_character->potion=2;

    //직업 설정
    while(check=='n'||check=='N'){
        system("cls");
        printf("직업을 골라주세요.\n");
        printf("1.검사 2.궁사 3.마법사\n");
        printf("선택: ");
        scanf("%d",&job);
        //검사
        if(job==1){
            //검사,hp:120,atk:20,def:30,mp:10, 고유 스킬: 베기 -> 참격
            system("cls");
            swordsman_image_print();
            printf("검사, 고유스킬: 베기->참격\n");
            printf("공격력과 방어력이 고르게 높음 밸런스형..\n");
            printf("정말로 검사를 고르시겠습니까?(y/n)\n");
            printf("선택: ");
            scanf(" %c",&check);
            if(check=='y'||check=='Y'){
                new_character->hp=120;
                new_character->atk=20;
                new_character->def=30;
                new_character->mp=10;
                new_character->basic_skill=1;
                new_character->random_skill=0;
                new_character->state=0;
            }
        //궁사
        }else if(job ==2){
            //궁사,hp:100,atk:50,def:20,mp:30, 고유 스킬: 활쏘기 -> 삼연 활쏘기
            system("cls");
            archer_image_print();
            printf("궁사, 고유스킬: 활쏘기-> 삼연 활쏘기\n");
            printf("공격력이 매우 높은 공격형..\n");
            printf("정말로 궁사를 고르시겠습니까?(y/n)\n");
            printf("선택: ");
            scanf(" %c",&check);
            if(check=='y'||check=='Y'){
                new_character->hp=100;
                new_character->atk=50;
                new_character->def=20;
                new_character->mp=30;
                new_character->basic_skill=2;
                new_character->random_skill=0;
                new_character->state=0;
            }
        //마법사
        }else{
            
            //마법사,hp:90,atk:20,def:20,mp:60, 고유 스킬: 매직미사일 -> 파이어볼
            system("cls");
            wizard_image_print();
            printf("마법사, 매직미사일->파이어볼\n");
            printf("마력이 높아 스킬에 큰 효과를 받을 수 있는 버프형..\n");
            printf("정말로 검사를 고르시겠습니까?(y/n)\n");
            printf("선택: ");
            scanf(" %c",&check);
            if(check=='y'||check=='Y'){
                new_character->hp=90;
                new_character->atk=20;
                new_character->def=20;
                new_character->mp=60;
                new_character->basic_skill=3;
                new_character->random_skill=0;
                new_character->state=0;
            }
            
        }
    }
    //완성된 캐릭터 확인
    system("cls");
    printf("캐릭터가 완성되었습니다.\n");
    printf("이름: %s\n",new_character->name);
    printf("HP: %3d     ATK: %3d   DEF: %3d     MP: %3d\n",new_character->hp,new_character->atk,new_character->def,new_character->mp);
    printf("계속할려면 엔터...");
    getchar();


    return new_character;
}

//싸우는 적의 모습과 상태, 플레이어의 상태를 알려주는 함수
void printUI(ch *user, ch *enemy){
    //적 모습 출력
    if(enemy->mob==1){
        print_basic_mob();
    }else if(enemy->mob==2){

    }else{

    }
    //적 상태를 출력
    printf("적: %s:\n",enemy->name);
    printf("HP: %3d     ATK: %3d   DEF: %3d     MP: %3d\n",enemy->hp,enemy->atk,enemy->def,enemy->mp);
    //플레이어의 상태를 출력
    printf("유저: %s\n",user->name);
    printf("HP: %3d     ATK: %3d   DEF: %3d     MP: %3d\n",user->hp,user->atk,user->def,user->mp);
    printf("포션 개수: %d\n",user->potion);
}

//고유스킬의 그래픽 출력과 설명을 해주는 함수
void basic_skill_ef(ch *target){
     /*고유 스킬:
        검사->  1. 베기     ->   4.참격
        궁사->  2. 활쏘기    ->  5.트리플샷
        마법사->3. 매직미사일 ->  6. 파이어볼

        적들 스킬:
        강아지-> 7. 물기
        케로베로스-> 8.많이 물기
        독수리-> 9. 날개치기
    */
   //베기
   if(target->basic_skill == 1){
    sword_1_attack_print();    
    printf("\"베기\"\n");
    printf("적을 검으로 베어버립니다.\n");
    printf("공격력: atk X 0.5~2");
    //활쏘기
   }else if(target->basic_skill ==2){
    arrow_1_attack_print();
    printf("\"활쏘기\"\n");
    printf("적을 향해 활을 쏴 공격합니다.\n");
    printf("공격력: atk X 0.5~1 + mp X 0.5~1");
    //매직미사일
   }else if(target->basic_skill==3){
    spell_1_attack_print();
    printf("\"매직미사일\"\n");
    printf("적을 향해 마법으로 미사일을 쏩니다.\n");
    printf("공격력: mp X 0~2.5");
    //물기
   }else if(target->basic_skill==7){
    printf("\"물기\"\n");
    printf("적이 당신을 물어버립니다!!\n");
    printf("공격력: atk X 0.8~2\n");
    mob_1_skill();
   }
}
//고유(공격) 스킬을 사용한다면 스탯에 따른 공격력을 알려주는 함수
int basic_skill(ch *target){
    srand(time(NULL));
    //베기
    if(target->basic_skill==1){
        return target->atk*((float)(rand()%15)/10+0.5);
    //활쏘기
    }else if(target->basic_skill==2){
        return target->atk*((float)(rand()%5)/10+0.5)+target->mp*((float)(rand()%5)/10+0.5);
    //매직미사일
    }else if(target->basic_skill==3){
        return target->mp*((float)(rand()%25)/10);
    //물기
    }else if(target->basic_skill==7){
        return target->atk*((float)(rand()%12)/10+0.8);
    }
}

//랜덤 기술의 그래픽과 설명을 해주는 함수
void random_skill_ef(ch *target){
   /* 랜덤 스킬:
        1. 회복
        2. 방패막
        3. 공격 상승
        4. 기절시키기
        5. 카운터
        6. mp 상승

        적들: 
        7. 짓기
        8. 웅크리기
        9. 날아오르기
    */

}

//랜덤 기술의 효과를 적용시키는 함수
void random_skill(ch *target){

}

//현재 플레이어가 가진 스킬들을 보여주고 스킬 선택 및 그에 따른 효과를 적용시키는 함수
void skillUI(ch *user,ch *enemy){    
    char choose ='n';
    int skill_choose;
    while(choose=='n'||choose=='N')
    {   
        //가지고 있는 스킬 출력
        system("cls");
        printf("%s님이 가지고 계신 스킬: \n",user->name);
        printf("고유 스킬: ");
        if(user->basic_skill==1){
            printf("베기\n");
        }else if(user->basic_skill==2){
            printf("활쏘기\n");
        }else if(user->basic_skill==3){
            printf("매직미사일\n");
        }else if(user->basic_skill==4){
            printf("참격\n");
        }else if(user->basic_skill==5){
            printf("트리플샷\n");
        }else if(user->basic_skill==6){
            printf("파이어볼\n");
        }
        printf("랜덤 스킬: ");
        if(user->random_skill==0){
            printf("없음\n");
        }else if(user->random_skill==1){
            printf("회복\n");
        }else if(user->random_skill==2){
            printf("방패막\n");
        }else if(user->random_skill==3){
            printf("공격 상승\n");
        }else if(user->random_skill==4){
            printf("기절시키기\n");
        }else if(user->random_skill==5){
            printf("카운터\n");
        }else if(user->random_skill==6){
            printf("mp 상승\n");
        }
        //스킬 사용
        printf("사용하실 스킬을 선택하세요.\n");
        printf("고유 스킬:1, 랜덤 스킬:2\n");
        printf("선택: ");
        scanf("%d",&skill_choose);
        //고유(공격) 스킬 사용시
        if(skill_choose==1){
            system("cls");
            basic_skill_ef(user);
            printf("이 스킬을 사용하시겠습니까?\n");
            printf("선택(y/n): ");
            scanf(" %c",&choose);
            if(choose=='y'||choose=='Y'){                
                int attack = basic_skill(user);
                //적의 hp을 공격력만큼 차감
                enemy->hp-=attack;
                printf("당신은 적에게 %d만큼 피해를 주었습니다.\n",attack);
            }
        //랜덤 스킬 사용시
        }else{
            system("cls");
            //스킬이 없을때
            if(user->random_skill==0){
                printf("스킬을 가지고 있지 않습니다.\n");
                printf("엔터를 눌러 다시 선택하세요.");
                getchar();                            
                getchar();
            }else{
                //랜덤 스킬에 따른 이벤트 적용
                random_skill_ef(user);
                printf("이 스킬을 사용하시겠습니까?\n");
                printf("선택(y/n): ");
                scanf(" %c",choose);
                if(choose=='y'||choose=='Y'){
                    random_skill(user);
                }
            }
            
        }
    }
    getchar();
    

}
//사용자의 선택을 받는 함수
int user_choose(ch *user, ch *enemy,int choose){
    //1은 스킬과 공격 선택
    if(choose==1){
        //위에 있는 함수로 넘어간다.
        skillUI(user,enemy);
        printf("엔터로 계속...");
        getchar();
        system("cls");
        return 0;
    //2는 방어 선택
    }else if(choose==2){
        //적의 행동을 나타내는 함수에서 처리
        defense_print();
        printf("%s는 방어를 준비합니다...!\n",user->name);
        printf("엔터로 계속...\n");
        getchar();
        getchar();
        system("cls");
        return 0;
    //3은 포션 사용
    }else{
        
        potion_print();
        //포션이 있다면...
        if(user->potion>0){
            //포션을 사용하기에 hp에 +20
            printf("%s는 포션을 사용해 HP를 회복합니다!\n");
            printf("HP: %d+20",user->hp);
            printf("남은 포션:%d \n",user->potion);
            user->hp+=20;
            user->potion-=1;
            printf("엔터로 계속...");
            getchar();
            getchar();
            system("cls");
            return 0;
        }else{
            //포션이 없다면 다시 선택
            printf("포션이 없습니다.....\n");
            printf("다시선택하세요...\n");
            printf("엔터로 계속...");
            getchar();
            getchar();
            system("cls");
            return 1;
        }
        
        
    }
    
    
}
//적의 행동을 결정하는 함수
void enemy_choose(ch *user,ch *enemy,int choose){
    srand(time(NULL));
    //랜덤으로 결정
    int enemy_choose = rand()%2+1;
    system("cls");
    //적이 고유(공격) 스킬을 씀
    if(enemy_choose==1){
        basic_skill_ef(enemy);
        printf("상대는 당신을 공격합니다....\n");
        int attack = basic_skill(enemy);
        //만약 플레이어가 방어를 선택했다면
        if(choose==2){
            printf("당신의 방어력: %d\n",user->def);
            printf("상대의 공격력: %d\n",attack);
            //적의 공격력이 플레이어의 방어력보다 높을때
            if(user->def<attack){
                printf("당신은 총 %d만큼 공격을 받았습니다!\n",attack-user->def);
                user->hp-=(attack-user->def);
            //플레이어의 방어력이 적의 공격력보다 높을때
            }else{
                printf("당신은 완벽히 상대의 공격을 막습니다!\n");
            }
            printf("엔터로 계속!");
            getchar();
        //플레이어가 방어가 아닌 다른 행동을 할때
        }else{
            printf("당신은 적에게 %d만큼 공격을 받습니다.\n",attack);
            user->hp-=attack;
            printf("엔터로 계속!");
            getchar();
        }
    //적이 랜덤 기술을 씀
    }else{        
        random_skill_ef(enemy);
        printf("상대는 스킬을 씁니다.\n");        
        random_skill(enemy);
        printf("엔터로 계속!");
        getchar();
    }
}

//적과 싸우는 이벤트 함수
void figth_event(ch *user,ch *enemy){
    int choose;
    //적 출현 이벤트
    system("cls");
    printf("이런! 야생의 %s가 싸움을 걸었다!",enemy->name);
    Sleep(1000);
    system("cls");
    spawn_mob_event();
    //적과 플레이어의 hp가 0보다 커야함
    while(user->hp>0&&enemy->hp>0){
        system("cls");
        char check = 'n';
        //플레이어와 적의 상태 출력
        printUI(user,enemy);
        while(check=='n'||check=='Y'){
            //플레이어의 행동을 입력받음
            printf("1. 공격&스킬 2.방어 3.포션 사용\n");
            printf("선택: ");
            scanf("%d",&choose);
            //한번더 확인
            if(choose==1){
                printf("공격&스킬을 선택하신게 맞나요?");
                printf("선택(y/n): ");
                scanf(" %c",&check);
            }else if(choose==2){
                printf("방어을 선택하신게 맞나요?");
                printf("선택(y/n): ");
                scanf(" %c",&check);
            }else{
                printf("포션 사용을 선택하신게 맞나요?");
                printf("선택(y/n): ");
                scanf(" %c",&check);
            }
            system("cls");
        }
        //위의 플레이어 행동 적용함수 처리
        if(user_choose(user,enemy,choose)==0){
            enemy_choose(user,enemy,choose);
        };
        //위의 적의 행동 적용함수 처리
        
    }
    
}
