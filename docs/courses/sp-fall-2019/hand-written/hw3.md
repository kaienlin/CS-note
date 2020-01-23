# Assignment 5

## Signal

In the following code, the child sends 10 signals to its parent and then the parent counts the total number.

1. What's the output of the parent? Explain your answer.

2. if fork() is replaced with vfork(), is your answer still the same? Why?

```c
int count = 0;
void handler(int sig)
{
    count++;
    return;
}
int main()
{
    int i, status;
    signal(SIGUSR1, handler);
    if ( fork() == 0 ) {
        for ( i = 0; i < 10; i++ )
            kill(getppid(), SIGUSR1);
        exit(0);
    }
    wait(&status);
    printf("Got %d signals\n", count);
}
```

## My Answer
**假設**：在執行signal handler之後該handler不會被reset成default action。

1. **count可能是1到10的任何一個數字。**
  因為若我們使用 fork(),便有可能在child和parent之間context switch。由於當parent不在CPU執行時，signal會被generate但不會被deliver，系統會將process table中的一個flag on起來表示這個signal是在pending的狀態。所以若是在這時重複generate該signal，在deliver時也還是只會有一次(因為flag只有on/off兩種狀態）。因此，child送給parent的10次SIG_USR1可能會有數次是這種情況，在極端情況下，child可能送完10次後才context switch到parent，如此parent就只會被deliver一次SIG_USR1，最終count為1。或者child每送一次SIG_USR1，就context switch到parent去執行signal handler，再回到child送下一次signal，這樣就會有10次deliver，則count為10。

    **附註**：若該系統在每次signal handler執行完後便會將該signal的action重設成default action，則handler()最多執行一次，且因為SIG_USR1的default action是terminate，所以若SIG_USR1發生第二次deliver，parent process便會被強制結束，而不會print "Got ? signals"。

2. **不，和(a)不同，count永遠是1。** 
因vfork()會suspend parent process直到child process return。所以child會完整送完10次SIG_USR1才會回到parent，由於系統處理pending signal的機制是把process table中的flag on或off，所以就算送了10次，對系統來說最後都是一個on的狀態而已，在回到parent時只會deliver一次就結束了。所以 handle()只會執行一次，故count最終必為1。