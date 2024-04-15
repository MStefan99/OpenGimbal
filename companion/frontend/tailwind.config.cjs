'use strict';

module.exports = {
	content: ['./index.html', './src/**/*.vue'],
	darkMode: 'media', // 'media' or 'class'
	theme: {
		// Override the theme here
		extend: {
			// Extend the theme here
			colors: {
				foreground: 'var(--color-foreground)',
				background: 'var(--color-background)',
				light: 'var(--color-light)',
				muted: 'var(--color-muted)',
				accent: 'var(--color-accent)',
				red: 'var(--color-red)',
				yellow: 'var(--color-yellow)',
				green: 'var(--color-red)',
				blue: 'var(--color-blue)'
			}
		}
	},
	plugins: []
};
