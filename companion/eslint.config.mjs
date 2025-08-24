import globals from 'globals';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
import js from '@eslint/js';
import {FlatCompat} from '@eslint/eslintrc';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const compat = new FlatCompat({
	baseDirectory: __dirname,
	recommendedConfig: js.configs.recommended,
	allConfig: js.configs.all
});

export default [
	{
		ignores: [
			'.vscode/*',
			'!.vscode/settings.json',
			'!.vscode/tasks.json',
			'!.vscode/launch.json',
			'!.vscode/extensions.json',
			'**/*.code-workspace',
			'**/.history/',
			'.idea/**/workspace.xml',
			'.idea/**/tasks.xml',
			'.idea/**/usage.statistics.xml',
			'.idea/**/dictionaries',
			'.idea/**/shelf',
			'.idea/**/contentModel.xml',
			'.idea/**/dataSources/',
			'.idea/**/dataSources.ids',
			'.idea/**/dataSources.local.xml',
			'.idea/**/sqlDataSources.xml',
			'.idea/**/dynamic.xml',
			'.idea/**/uiDesigner.xml',
			'.idea/**/dbnavigator.xml',
			'.idea/**/gradle.xml',
			'.idea/**/libraries',
			'**/cmake-build-*/',
			'.idea/**/mongoSettings.xml',
			'**/*.iws',
			'**/out/',
			'**/.idea_modules/',
			'**/atlassian-ide-plugin.xml',
			'.idea/replstate.xml',
			'**/com_crashlytics_export_strings.xml',
			'**/crashlytics.properties',
			'**/crashlytics-build.properties',
			'**/fabric.properties',
			'.idea/httpRequests',
			'.idea/caches/build_file_checksums.ser',
			'**/logs',
			'**/*.log',
			'**/npm-debug.log*',
			'**/yarn-debug.log*',
			'**/yarn-error.log*',
			'**/lerna-debug.log*',
			'**/report.[0-9]*.[0-9]*.[0-9]*.[0-9]*.json',
			'**/pids',
			'**/*.pid',
			'**/*.seed',
			'**/*.pid.lock',
			'**/lib-cov',
			'**/coverage',
			'**/*.lcov',
			'**/.nyc_output',
			'**/.grunt',
			'**/bower_components',
			'**/.lock-wscript',
			'build/Release',
			'**/node_modules/',
			'**/jspm_packages/',
			'**/web_modules/',
			'**/*.tsbuildinfo',
			'**/.npm',
			'**/.eslintcache',
			'**/.rpt2_cache/',
			'**/.rts2_cache_cjs/',
			'**/.rts2_cache_es/',
			'**/.rts2_cache_umd/',
			'**/.node_repl_history',
			'**/*.tgz',
			'**/.yarn-integrity',
			'**/.env',
			'**/.env.test',
			'**/.cache',
			'**/.parcel-cache',
			'**/.next',
			'**/out',
			'**/.nuxt',
			'**/dist',
			'**/.cache/',
			'.vuepress/dist',
			'**/.serverless/',
			'**/.fusebox/',
			'**/.dynamodb/',
			'**/.tern-port',
			'**/.vscode-test',
			'.yarn/cache',
			'.yarn/unplugged',
			'.yarn/build-state.yml',
			'.yarn/install-state.gz',
			'**/.pnp.*',
			'docs/_book',
			'**/test/',
			'**/package-lock.json',
			'**/*.css',
			'**/backend'
		]
	},
	...compat.extends(
		'eslint:recommended',
		'plugin:@typescript-eslint/recommended',
		'plugin:vue/vue3-essential',
		'plugin:vue-pug/vue3-recommended'
	),
	{
		languageOptions: {
			globals: {
				...globals.browser
			},

			ecmaVersion: 'latest',
			sourceType: 'module',

			parserOptions: {
				parser: '@typescript-eslint/parser'
			}
		},

		rules: {
			'@typescript-eslint/no-unused-vars': 'off',
			'@typescript-eslint/no-inferrable-types': 'off',
			'@typescript-eslint/explicit-function-return-type': 'error',
			'@vue/multi-word-components': 'off',
			'@typescript-eslint/no-unused-expressions': ['error', {allowShortCircuit: true}]
		}
	}
];
