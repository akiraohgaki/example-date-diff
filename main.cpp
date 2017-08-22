#include <iostream>

using namespace std;

namespace date {

const int daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // in 24 months
const int elapsedDays[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}; // in 12 months

struct Date {
    int year;
    int month;
    int day;
};

class Diff {
public:
    explicit Diff(int startYear = 1, int startMonth = 1, int startDay = 1, int endYear = 1, int endMonth = 1, int endDay = 1)
    {
        setStartDate(startYear, startMonth, startDay);
        setEndDate(endYear, endMonth, endDay);
    }

    Date startDate() const
    {
        return startDate_;
    }

    void setStartDate(int year, int month, int day)
    {
        startDate_.year = year;
        startDate_.month = month;
        startDate_.day = day;
    }

    Date endDate() const
    {
        return endDate_;
    }

    void setEndDate(int year, int month, int day)
    {
        endDate_.year = year;
        endDate_.month = month;
        endDate_.day = day;
    }

    int calcLeap() const
    {
        return calcLeapFromDate(endDate_.year, endDate_.month, endDate_.day) - calcLeapFromDate(startDate_.year, startDate_.month, startDate_.day);
    }

    int calcDays() const
    {
        return calcDaysFromDate(endDate_.year, endDate_.month, endDate_.day) - calcDaysFromDate(startDate_.year, startDate_.month, startDate_.day);
    }

    Date calcDate() const
    {
        Date diffDate;
        diffDate.year = 0;
        diffDate.month = 0;
        diffDate.day = 0;

        int days = calcDays() - calcLeap(); // make days into common year

        if (days >= 365) {
            diffDate.year = days / 365;
        }
        days -= 365 * diffDate.year;

        int startMonth = 0;
        if (startDate_.year == endDate_.year) {
            startMonth = startDate_.month;
        }
        else if (startDate_.year < endDate_.year) {
            startMonth = endDate_.month;
        }
        if (startMonth > 0) {
            for (int i = startMonth - 1; daysPerMonth[i] <= days; i++) {
                diffDate.month++;
                days -= daysPerMonth[i];
            }
            diffDate.day = days;
        }

        return diffDate;
    }

private:
    int calcLeapInJulianCalendarFromYear(int year) const
    {
        return year / 4;
    }

    int calcLeapInGregorianCalendarFromYear(int year) const
    {
        return (year / 4) - (year / 100) + (year / 400);
    }

    int calcLeapFromDate(int year, int month, int day) const
    {
        // Julian calendar 1582/10/4 -> dropped 10 days -> Gregorian calendar 1582/10/15
        if (year < 1582 || (year == 1582 && month < 10) || (year == 1582 && month == 10 && day < 4)) {
            return calcLeapInJulianCalendarFromYear(year);
        }
        return calcLeapInJulianCalendarFromYear(1582) - 10 + calcLeapInGregorianCalendarFromYear(year) - calcLeapInGregorianCalendarFromYear(1582);
    }

    int calcDaysFromDate(int year, int month, int day) const
    {
        const int dYear = 365 * (year - 1);
        const int dLeap = calcLeapFromDate(year, month, day);
        const int dMonth = elapsedDays[month - 1];
        return dYear + dLeap + dMonth + day - 1;
    }

    Date startDate_;
    Date endDate_;
};

}

int main(int argc, char *argv[])
{
    date::Diff dateDiff(1997, 8, 19, 2017, 3, 9);

    date::Date startDate = dateDiff.startDate();
    date::Date endDate = dateDiff.endDate();

    printf("Start date: %d/%d/%d \n", startDate.year, startDate.month, startDate.day);
    printf("End date: %d/%d/%d \n", endDate.year, endDate.month, endDate.day);

    date::Date diffDate = dateDiff.calcDate();
    int diffDays = dateDiff.calcDays();

    printf("Duration between two dates: %d years %d months %d days \n", diffDate.year, diffDate.month, diffDate.day);
    printf("Or: %d days \n", diffDays);

    return 0;
}
