export function traverse(object: any, propertyName: string | symbol): any {
	const properties = propertyName.toString().split('/');
	for (const property of properties) {
		if (!(property in object)) {
			throw new Error('Property "' + property + '" is not defined in', object);
		} else {
			object = object[property];
		}
	}
	return object;
}
