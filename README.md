[![Build Status](https://travis-ci.org/hhvm/hh-clilib.svg?branch=master)](https://travis-ci.org/hhvm/hh-clilib)

# Hack CLI Library

This library provides basic command-line handling, including:
- parsing of `ARGV`
- interactive TTY detection
- color TTY detection

It aims for as much code as possible to be in strict mode.

## Installation

```
hhvm composer.phar require facebook/hh-clilib
```

## Examples

In `src/MyCLI.hh`:

```Hack
// MyCLI.hh
<?hh // strict

use type Facebook\CLILib\CLIBase;

final class MyCLI extends CLIBase {
  <<__Override>>
  public async function mainAsync(): Awaitable<int> {
    $this->getStdout()->write("Hello, world!');
    return 0;
  }
}
```

In `bin/mycli`:

```Hack
<?hh // not strict because of top-level statements.

require_once(__DIR__.'/../vendor/hh_autoload.php');

MyCLI::main();
```

## Options

Options are optional, always have a long form (e.g. `--foo`), may have a short form (e.g. `-f`), and may
require a value (e.g. `--foo=bar` or `--foo bar`).

You can specify supported options by implemented `getSupportedOptions()`; `--help` is always supported.

```Hack
<<__Override>>
protected function getSupportedOptions(): vec<CLIOptions\CLIOption> {
	return vec[
		CLIOptions\flag(
			() ==> { $this->verbosity++; },
			"Increase output verbosity",
			'--verbose',
			'-v',
		),
		CLIOptions\with_required_enum(
			OutputFormat::class,
			$f ==> { $this->format = $f; },
			Str\format(
				"Desired output format (%s). Default: %s",
				Str\join(OutputFormat::getValues(), '|'),
				(string) $this->format,
			),
			'--format',
			'-f',
		),
		CLIOptions\with_required_string(
			$s ==> { $this->outputRoot = $s; },
			"Directory for output files. Default: working directory",
			'--output',
			'-o',
		),
	];
}
```

## Arguments

Arguments do not have a name, and may be required. To support arguments, extend `CLIWithArguments` or `CLIWithRequiredArguments`.

Arguments are always strings, and can be retrieved via `->getArguments();`

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## License

hh-clilib is MIT-licensed.
