<?php

const ITERATIONS = 1_000_000;


/**
 * Benchmark runner
 */
function benchmark(string $label, callable $fn): void
{
    gc_collect_cycles();
    gc_disable();

    $memStart = memory_get_usage(true);
    $timeStart = hrtime(true);

    for ($i = 0; $i < ITERATIONS; $i++) {
        $fn();
    }

    $timeEnd = hrtime(true);
    $memEnd = memory_get_usage(true);

    gc_enable();

    printf(
        "%-30s %8.2f ms   %+7.2f KB\n",
        $label,
        ($timeEnd - $timeStart) / 1e6,
        ($memEnd - $memStart) / 1024
    );
}

$json = '{"a":1,"b":2,"c":[1,2,3]}';

benchmark('poly json_validate()', fn() =>
    Php83::json_validate($json)
);

benchmark('Corefill json_validate()', fn() =>
    json_validate($json)
);



final class Php83
{
    private const JSON_MAX_DEPTH = 0x7FFFFFFF; // see https://www.php.net/manual/en/function.json-decode.php

    public static function json_validate(string $json, int $depth = 512, int $flags = 0): bool
    {
        if (0 !== $flags && \defined('JSON_INVALID_UTF8_IGNORE') && \JSON_INVALID_UTF8_IGNORE !== $flags) {
            throw new \ValueError('json_validate(): Argument #3 ($flags) must be a valid flag (allowed flags: JSON_INVALID_UTF8_IGNORE)');
        }

        if ($depth <= 0) {
            throw new \ValueError('json_validate(): Argument #2 ($depth) must be greater than 0');
        }

        if ($depth > self::JSON_MAX_DEPTH) {
            throw new \ValueError(sprintf('json_validate(): Argument #2 ($depth) must be less than %d', self::JSON_MAX_DEPTH));
        }

        json_decode($json, true, $depth, $flags);

        return \JSON_ERROR_NONE === json_last_error();
    }
}