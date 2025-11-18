import {defineConfig} from 'vitepress';

// https://vitepress.dev/reference/site-config
export default defineConfig({
    title: 'OpenGimbal',
    description: 'OpenGimbal documentation',
    themeConfig: {
        // https://vitepress.dev/reference/default-theme-config
        nav: [
            {text: 'Home', link: '/'},
            {text: 'Get started', link: '/intro'},
            {text: 'Companion', link: '/companion', target: '_self'},
        ],

        sidebar: [
            {
                text: 'Getting started',
                items: [
                    {text: 'Introduction', link: '/intro'},
                    {text: 'Hardware', link: '/hardware'},
                    {text: 'Software', link: '/software'},
                    {text: 'Build yours', link: '/build'}
                ]
            }
        ],

        socialLinks: [{icon: 'github', link: 'https://github.com/MStefan99/OpenGimbal'}]
    },
    cleanUrls: true,
    head: [
        ...(process.env.VITE_CRASH_COURSE_URL ? [['script', {
            async: '',
            type: 'module',
            src: `${process.env.VITE_CRASH_COURSE_URL}/cc?k=${process.env.VITE_CRASH_COURSE_KEY}&auto`
        }]] : [])
    ]
});
