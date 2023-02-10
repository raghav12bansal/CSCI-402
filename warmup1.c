    /*
     * Author:      William Chia-Wei Cheng (bill.cheng@usc.edu)
     *
     * @(#)$Id: listtest.c,v 1.2 2020/05/18 05:09:12 william Exp $
     */

    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <time.h>
    #include "cs402.h"

    #include "my402list.h"
    
    void reverse(char*);
    void printOutput(My402List *myList, int num_items);
    void printSortedList(My402List *myList, int num_items);
    typedef struct transactionRecords{

        int timestamp;
        char processedDate[16];
        char description[25];
        char amountL[20];
        char amountR[20];
        char sign;
    }transaction;

    void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
        /* (*pp_elem1) must be closer to First() than (*pp_elem2) */
    {
        My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
        void *obj1=elem1->obj, *obj2=elem2->obj;
        My402ListElem *elem1prev=My402ListPrev(pList, elem1);
    /*  My402ListElem *elem1next=My402ListNext(pList, elem1); */
    /*  My402ListElem *elem2prev=My402ListPrev(pList, elem2); */
        My402ListElem *elem2next=My402ListNext(pList, elem2);

        My402ListUnlink(pList, elem1);
        My402ListUnlink(pList, elem2);
        if (elem1prev == NULL) {
            (void)My402ListPrepend(pList, obj2);
            *pp_elem1 = My402ListFirst(pList);
        } else {
            (void)My402ListInsertAfter(pList, obj2, elem1prev);
            *pp_elem1 = My402ListNext(pList, elem1prev);
        }
        if (elem2next == NULL) {
            (void)My402ListAppend(pList, obj1);
            *pp_elem2 = My402ListLast(pList);
        } else {
            (void)My402ListInsertBefore(pList, obj1, elem2next);
            *pp_elem2 = My402ListPrev(pList, elem2next);
        }
    }

    void BubbleSortForwardList(My402List *pList, int num_items)
    {
        My402ListElem *elem=NULL;
        int i=0;

        if (My402ListLength(pList) != num_items) {
            fprintf(stderr, "List length is not %1d in BubbleSortForwardList().\n", num_items);
            exit(1);
        }
        for (i=0; i < num_items; i++) {
            int j=0, something_swapped=FALSE;
            My402ListElem *next_elem=NULL;

            for (elem=My402ListFirst(pList), j=0; j < num_items-i-1; elem=next_elem, j++) {
                next_elem=My402ListNext(pList, elem);
                int cur_val= (int)((transaction *)(elem->obj))->timestamp, next_val = (int)((transaction *)(next_elem->obj))->timestamp;
                if (cur_val > next_val){
                    BubbleForward(pList, &elem, &next_elem);
                    something_swapped = TRUE;
                }
            }
            if (!something_swapped) break;
        }
    }

    void BubbleBackward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
        /* (*pp_elem1) must be closer to Last() than (*pp_elem2) */
    {
        My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
        void *obj1=elem1->obj, *obj2=elem2->obj;
    /*  My402ListElem *elem1prev=My402ListPrev(pList, elem1); */
        My402ListElem *elem1next=My402ListNext(pList, elem1);
        My402ListElem *elem2prev=My402ListPrev(pList, elem2);
    /*  My402ListElem *elem2next=My402ListNext(pList, elem2); */

        My402ListUnlink(pList, elem1);
        My402ListUnlink(pList, elem2);
        if (elem1next == NULL) {
            (void)My402ListAppend(pList, obj2);
            *pp_elem1 = My402ListLast(pList);
        } else {
            (void)My402ListInsertBefore(pList, obj2, elem1next);
            *pp_elem1 = My402ListPrev(pList, elem1next);
        }
        if (elem2prev == NULL) {
            (void)My402ListPrepend(pList, obj1);
            *pp_elem2 = My402ListFirst(pList);
        } else {
            (void)My402ListInsertAfter(pList, obj1, elem2prev);
            *pp_elem2 = My402ListNext(pList, elem2prev);
        }
    }

    void BubbleSortBackwardList(My402List *pList, int num_items)
    {
        My402ListElem *elem=NULL;
        int i=0;

        if (My402ListLength(pList) != num_items) {
            fprintf(stderr, "List length is not %1d in BubbleSortBackwardList().\n", num_items);
            exit(1);
        }
        for (i=0; i < num_items; i++) {
            int j=0, something_swapped=FALSE;
            My402ListElem *prev_elem=NULL;

            for (elem=My402ListLast(pList), j=0; j < num_items-i-1; elem=prev_elem, j++) {
                int cur_val=(int)(elem->obj), prev_val=0;

                prev_elem=My402ListPrev(pList, elem);
                prev_val = (int)(prev_elem->obj);

                if (cur_val < prev_val) {
                    BubbleBackward(pList, &elem, &prev_elem);
                    something_swapped = TRUE;
                }
            }
            if (!something_swapped) break;
        }
    }

    /* ----------------------- main() ----------------------- */

    int main(int argc, char *argv[])
    {
        
        if(strcmp(argv[1], "sort"))
        {
            printf("Malformed commandline - \"%s\" is not a valid commandline option.\n", argv[1]);
            printf("usage: warmup1 sort [tfile]\n");
            return(0);  
        }
        else if(argc<2 || argc>3)
        {
            printf("Malformed commandline - Incorrect commandline arguments.\n");
            printf("usage: warmup1 sort [tfile]\n");
            return(0);
        }
        else
        {
            FILE *fp;
            if(argc==3)
            {
                fp = fopen(argv[2], "r");
                if(!fp)
                {
                    printf("ERROR:File \"%s\" not found\n", argv[2]);
                    return(0);
                }
            }
            else
            {
                fp = stdin;
            }
            char buf[2000];
            My402List myList;
            int totalRecords = 0;
            memset(&myList, 0, sizeof(My402List));
            while(fgets(buf, sizeof(buf), fp)) 
            {
                if(strcmp(buf,"\n")==0)
                {
                    break;
                }
                totalRecords++;
                int num_tabs = 0;
                int indexArray[100];
                int stringLength = 0;
                for(int i=0;i<sizeof(buf);i++)
                {
                    stringLength++;
                    if(buf[i]=='\0')
                    {
                        stringLength--;
                        break;
                    }
                    else if(buf[i]=='\t')
                    {
                        indexArray[num_tabs] = i;
                        num_tabs++;
                        buf[i] = '\0';
                                            }
                    if(stringLength>1026)
                    {
                        printf("ERROR: The length of input on line %d is greater than 1024.\n", totalRecords);
                        return(0);
                    }
                }
                if(num_tabs>3)
                {
                    printf("ERROR: The number of fields on line %d are greater than 3. It should have exactly 3 tabs.\n", totalRecords);
                    return(0);
                }
                if(num_tabs<3)
                {
                    printf("ERROR: The number of fields on line %d are less than 3. It should have exactly 3 tabs.\n", totalRecords);
                    return(0);
                }
                char description[1024];
                char amount[80];
                char date[20];
                char sign;
                if(stringLength>1024)
                {
                    printf("ERROR: The length of input on line %d is greater than 1024.\n", totalRecords);
                    return(0);
                }
                if(buf[0]!='+'&&buf[0]!='-')
                {
                    printf("ERROR: The sign for the transaction on line %d is invalid.\n", totalRecords);
                    return(0);
                }
                sign = buf[0];
                int count = 0;
                for(int i=indexArray[0]+1;i<indexArray[1];i++)
                {
                    if(buf[i]<'0'||buf[i]>'9')
                    {
                        printf("ERROR: The numbers in the date on line %d are not in the range 0-9.\n", totalRecords);
                        return(0);
                    }
                    date[count] = buf[i];
                    count++;
                    if(count>13)
                    {
                        printf("ERROR: The length of timestamp is greater than 13 on line %d\n", totalRecords);
                        return(0);
                    }
                }
                date[count] = '\0';
                if(date[0]=='0')
                {
                    printf("ERROR: First digit cannot be zero in timestamp on line %d\n", totalRecords);
                    return(0);
                }
                count = 0;
                int countOfDots = 0;
                for(int i=indexArray[1]+1;i<indexArray[2];i++)
                {
                    if(buf[i]=='.')
                    {
                        countOfDots++;
                    }
                    else if(buf[i]<'0'||buf[i]>'9')
                    {
                        printf("ERROR: The numbers in the transaction amount on line %d are not in the range 0-9.\n", totalRecords);
                        return(0);
                    }
                    amount[count] = buf[i];
                    count++;
                }
                if(countOfDots!=1)
                {
                    printf("ERROR: The count of period in the number on line %d is not equal to 1.\n", totalRecords);
                    return(0);
                }
                amount[count] = '\0';

                count = 0;
                for(int i=indexArray[2]+1;buf[i]!='\0';i++)
                {
                    description[count] = buf[i];
                    count++;
                }
                description[count] = '\0';
                int descriptionLength = count;
                int i = 0;
                while(i<descriptionLength&&description[i]==' ')
                {
                    i++;
                }
                if(i==descriptionLength||description[i]=='\n')
                {
                    printf("ERROR: Description is empty on line %d.\n", totalRecords);
                    return(0);
                }
                count = 0;

                char descriptionString[1024];
                while(i<descriptionLength)
                {
                    descriptionString[count++] = description[i++];
                }
                descriptionString[count] = '\0';

                descriptionLength = count;
                
                char amountL[20];
                char amountR[20];
                i = 0;
                while(amount[i]!='.')
                {
                    amountL[i] = amount[i];
                    i++;
                    if(i>7)
                    {
                        printf("ERROR: The integral part of transaction amount cannot exceed 7 characters on line %d\n", totalRecords);
                        return(0);
                    }
                }
                amountL[i] = '\0';
                i++;
                count = 0;
                while(amount[i]!='\0')
                {
                    amountR[count++] = amount[i++];
                    if(count>2)
                    {
                        printf("ERROR: The decimal part on line %d have more than two digits\n", totalRecords);
                        return(0);
                    }
                }
                amountR[count] = '\0';
                if(count<2)
                {
                    printf("ERROR: The decimal part on line %d have less than two digits\n", totalRecords);
                    return(0);
                }
                if(atoi(amountL)>=10000000)
                {
                    printf("ERROR: The amount for the transaction has to be less than 10 million.\n");
                    return(0);
                }
                if(atoi(amountL)>0&&amountL[0]=='0')
                {
                    printf("ERROR: First digit cannot be zero in non-zero amount on line %d\n", totalRecords);
                    return(0);
                }
                char descriptionProcessed[24];
                i = 0;
                while(i<24&&descriptionString[i]!='\0'&&descriptionString[i]!='\n')
                {
                    descriptionProcessed[i] = descriptionString[i];
                    i++;
                }
                while(i<24)
                {
                    descriptionProcessed[i] = ' ';
                    i++;
                }
                descriptionProcessed[i] = '\0';

                int dateTxn = atoi(date);
                time_t currentTime = time(NULL);
                time_t dateTimeVar = (time_t)dateTxn;
                if(dateTimeVar>currentTime)
                {
                    printf("ERROR: The timestamp on line %d cannot be greater than current timestamp\n", totalRecords);
                    return(0);
                }
                char* dateTime = (char*)ctime(&dateTimeVar);
                char processedDate[16];
                i = 0;
                while(i<10)
                {
                    processedDate[i] = dateTime[i];
                    i++;
                }
                count = 10;
                i = 19;
                while(i<24)
                {
                    processedDate[count] = dateTime[i];
                    i++;
                    count++;
                }
                processedDate[count] = '\0';
                

                transaction* txn = (transaction*)malloc(sizeof(transaction));
                txn->timestamp = dateTxn;
                txn->sign = sign;
                strcpy(txn->processedDate, processedDate);
                strcpy(txn->amountR, amountR);
                strcpy(txn->amountL, amountL);
                strcpy(txn->description, descriptionProcessed);
                
                My402ListElem *elem_ptr=NULL;
                for (elem_ptr=My402ListFirst(&myList); elem_ptr != NULL; elem_ptr=My402ListNext(&myList, elem_ptr))
                {
                    if((int)((transaction *)(elem_ptr->obj))->timestamp == dateTxn)
                    {
                        printf("ERROR: Two transactions cannot have same timestamp. Similar value found on line %d. \n", totalRecords);
                        return(0);
                    }
                }

                (void)My402ListAppend(&myList, txn);
            }
            fclose(fp);
            if(totalRecords==0)
            {
                printf("ERROR: The file \"%s\" is empty.\n", argv[2]);
                return(0);
            }
            int num_items = My402ListLength(&myList);
            BubbleSortForwardList(&myList, My402ListLength(&myList));
            printOutput(&myList, num_items);
        }
        return(0);
    }   
void reverse(char *s) {
    int length, c;
    char *begin, *end, temp;
    length = strlen(s);
    begin  = s;
    end    = s;
 
    for (c = 0; c < length - 1; c++)
        end++;
 
    for (c = 0; c < length/2; c++) {        
        temp   = *end;
        *end   = *begin;
        *begin = temp;
 
        begin++;
        end--;
    }
}
void printSortedList(My402List *myList, int num_items)
{
    My402ListElem *elem_ptr=NULL;
    int index = 0;
    long double balance = 0.0;

    for (elem_ptr=My402ListFirst(myList); index!=num_items; elem_ptr=My402ListNext(myList, elem_ptr)) 
    {
        double value = 0.0;
        int l = atoi(((transaction *)(elem_ptr->obj))->amountL);
        int r = atoi(((transaction *)(elem_ptr->obj))->amountR);
        char amountL[20], amountR[20];
        strcpy(amountL, (((transaction *)(elem_ptr->obj))->amountL));
        strcpy(amountR, (((transaction *)(elem_ptr->obj))->amountR));
        char sign = (((transaction *)(elem_ptr->obj))->sign);
        value = l + (r/100.0);
        if(sign=='-')
        {
            value = -value;
        }
        balance = balance + value;

        char newAmount[20];
        strcpy(newAmount, amountL);
        reverse(newAmount);
        char processedAmount[20];
        int j = 0;
        for(int i=0;i<strlen(newAmount);i++)
        {
            if(i%3==0&&i!=0)
            {
                processedAmount[j++] = ',';
            }
            processedAmount[j++] = newAmount[i];
        }
        processedAmount[j] = '\0';
        reverse(processedAmount);
        char finalAmount[20];
        j = 0;
        for(;j<strlen(processedAmount);j++)
        {
            finalAmount[j] = processedAmount[j];
        }
        finalAmount[j++] = '.';
        finalAmount[j++] = amountR[0];
        finalAmount[j++] = amountR[1];
        finalAmount[j] = '\0';
        char balanceAmount[15];
        balanceAmount[14] = '\0';
        for(int i=0;i<14;i++)
        {
            balanceAmount[i] = ' ';
        }
        balanceAmount[0] = ' ';
        balanceAmount[13] = ' ';
        if(sign=='-')
        {
            balanceAmount[0] = '(';
            balanceAmount[13] = ')';
        }
        j = 12;
        int count = strlen(finalAmount);
        while(count>0)
        {
            balanceAmount[j--] = finalAmount[count-1];
            count--;
        }

        long double originalBalance = balance;
        long double originalBalanceCopy = balance;
        char balance2[100];
        int ind = 0;
        char balanceL[20], balanceR[20];
        char signBalance=' ';
        if(originalBalance<0)
        {
            balance = -balance;
            signBalance = '-';
        }
        sprintf(balance2, "%Lf", balance);
        while(balance2[ind]!='.')
        {
            balanceL[ind] = balance2[ind];
            ind++;
        }
        balanceL[ind] = '\0';
        ind++;
        balanceR[0] = balance2[ind++];
        balanceR[1] = balance2[ind++];
        balanceR[2] = '\0';
        char newAmount2[20];
        strcpy(newAmount2, balanceL);
        reverse(newAmount2);
        char processedAmount2[20];
        j = 0;
        for(int i=0;i<strlen(newAmount2);i++)
        {
            if(i%3==0&&i!=0)
            {
                processedAmount2[j++] = ',';
            }
            processedAmount2[j++] = newAmount2[i];
        }
        processedAmount2[j] = '\0';
        reverse(processedAmount2);
        char finalAmount2[20];
        j = 0;
        for(;j<strlen(processedAmount2);j++)
        {
            finalAmount2[j] = processedAmount2[j];
        }
        finalAmount2[j++] = '.';
        finalAmount2[j++] = balanceR[0];
        finalAmount2[j++] = balanceR[1];
        finalAmount2[j] = '\0';
        char balanceAmount2[15];
        balanceAmount2[14] = '\0';
        for(int i=0;i<14;i++)
        {
            balanceAmount2[i] = ' ';
        }
        if(signBalance=='-')
        {
            balanceAmount2[0] = '(';
            balanceAmount2[13] = ')';
        }
        j = 12;
        int count2 = strlen(finalAmount2);
        while(count2>0)
        {
            balanceAmount2[j--] = finalAmount2[count2-1];
            count2--;
        }
        if(originalBalanceCopy<0)
        {
            originalBalanceCopy = -originalBalanceCopy;
        }
        if(originalBalanceCopy>=10000000)
        {
            char* outputBalance;
            if(signBalance != '-')
            {
                outputBalance = " ?,???,???.?? ";
            }
            else
            {
                outputBalance = "(\?,\?\?\?,\?\?\?.\?\?)";
            }
            printf("| %s | %s | %s | %s |\n", ((transaction *)(elem_ptr->obj))->processedDate, ((transaction *)(elem_ptr->obj))->description, balanceAmount, outputBalance);
        }
        else
        {
            printf("| %s | %s | %s | %s |\n", ((transaction *)(elem_ptr->obj))->processedDate, ((transaction *)(elem_ptr->obj))->description, balanceAmount, balanceAmount2);
        }
        index++;
        balance = originalBalance;
    }
}
void printOutput(My402List *myList, int num_items)
{
    printf("+-----------------+--------------------------+----------------+----------------+\n");
    printf("|       Date      | Description              |         Amount |        Balance |\n");
    printf("+-----------------+--------------------------+----------------+----------------+\n");
    printSortedList(myList, num_items);
    printf("+-----------------+--------------------------+----------------+----------------+\n");
}
