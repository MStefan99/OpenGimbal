'use strict';

module.exports = {
	content: ['./index.html', './src/**/*.vue'],
	darkMode: 'media', // 'media' or 'class'
	theme: {
		// Override the theme here
		extend: {
			// Extend the theme here
			colors: {
				white: 'var(--color-white)',
				black: 'var(--color-black)',
				gray: 'var(--color-gray)',
				background: 'var(--color-background)',
				foreground: 'var(--color-foreground)',
				muted: 'var(--color-muted)',
				light: 'var(--color-light)',
				accent: 'var(--color-accent)',
				red: 'var(--color-red)',
				orange: 'var(--color-orange)',
				yellow: 'var(--color-yellow)',
				green: 'var(--color-green)',
				'red-bright': 'var(--color-red-bright)',
				'orange-bright': 'var(--color-orange-bright)',
				'yellow-bright': 'var(--color-yellow-bright)',
				'green-bright': 'var(--color-green-bright)',
				'blue-bright': 'var(--color-blue-bright)',
				blue: 'var(--color-blue)',
				overlay: 'var(--color-overlay)',
				shadow: 'var(--color-shadow)'
			}
		}
	},
	plugins: []
};
