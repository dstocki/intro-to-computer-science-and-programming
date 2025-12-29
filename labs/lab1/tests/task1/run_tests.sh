#! /bin/bash

BIN="$1"
DIR="$(dirname "$0")"
DIR_LOGS="$DIR/logs"

green() {
    MSG="$1"
    printf "\033[32m%s\033[0m" "$MSG"
}

red() {
    MSG="$1"
    printf "\033[31m%s\033[0m" "$MSG"
}

fail() {
    REASON="$1"
    printf "["
    red "FAIL"
    printf "] ($REASON)\n"
}

pass() {
    printf "[OK]\n"
}

summ() {
    N_PASS="$1"
    N_FAIL="$2"
    N_ALL="$3"
    printf "*** SUMMARY: $N_ALL run, "
    green "$N_PASS passed"
    printf ", "
    if [ "$N_PASS" -eq "$N_ALL" ]; then
        printf "0 failed"
    else
        red "$N_FAIL failed"
    fi
    printf " ***\n"
}

if [ -z "$BIN" ] || [ ! -x "$BIN" ]; then
    echo "Usage: $0 <bin>"
    exit 2
fi

mkdir -p "$DIR_LOGS"
FAILED=0
PASSED=0
AMOUNT=0

for TEST in "$DIR"/*.txt; do
    NAME="$(basename "$TEST")"
    NAME_WO_EXT="${NAME%.txt}"
    printf "Running: $NAME_WO_EXT "

    # 1. Read testcase parameters
    INPUT=$(sed -n '/^INPUT:/,/^RESULT:/p' "$TEST" | sed '1d;$d')
    RESULT=$(sed -n '/^RESULT:/,/OUTPUT:/p' "$TEST" | sed '1d;$d' | \
        tr -d '[:space:]')
    EXPECTED=$(sed -n '/^OUTPUT:/,$p' "$TEST" | sed '1d')

    # 2. Run testcase and gather actual output
    ACTUAL=$(printf "%s\n" "$INPUT" | "$BIN")
    EXIT_CODE=$?

    ((AMOUNT++))

    # 3. Check if testcase exits with expected success
    if [ "$RESULT" = "success" ] && [ "$EXIT_CODE" -ne 0 ]; then
        fail "false negative"
        ((FAILED++))
        continue
    fi

    # 4. Check if testcase exits with expected failure
    if [ "$RESULT" = "failure" ] && [ "$EXIT_CODE" -eq 0 ]; then
        fail "false possitive"
        ((FAILED++))
        continue
    fi

    # 5. Validate actual vs expected diff if testcase exits with success
    if [ "$RESULT" = "success" ]; then
        DIFF=$(diff -u <(printf "%s\n" "$ACTUAL") <(printf "%s\n" "$EXPECTED"))
        DIFF_EXIT_CODE=$?

        if [ "$DIFF_EXIT_CODE" -ne 0 ]; then
            echo "ACTUAL (-) vs EXPECTED (+)" > "$DIR_LOGS/$NAME_WO_EXT.diff"
            echo "$DIFF" >> "$DIR_LOGS/$NAME_WO_EXT.diff"
            fail "difference in output"
            ((FAILED++))
            continue
        fi
    fi

    # 6. Testcase is passed
    ((PASSED++))
    pass
done

summ "$PASSED" "$FAILED" "$AMOUNT"

exit $FAILED
