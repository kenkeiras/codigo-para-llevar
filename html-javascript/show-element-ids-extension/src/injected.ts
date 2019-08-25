(function() {
    const elements_with_ids = document.querySelectorAll('*[id]') as any as HTMLElement[];
    const stylesheet = document.styleSheets[0] as CSSStyleSheet;

    const mark_element = function(element) {
        const rule = '#' + element.id + '::after{ content:" #' + element.id + '"; }';
        stylesheet.insertRule(rule, stylesheet.cssRules.length);
    };

    for (const element of elements_with_ids) {
        mark_element(element);
    }
})();
